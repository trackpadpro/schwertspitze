#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include "steam_api.h"
#include "object.h"

#if !defined(DEDICATED_SERVER)
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include "client.h"
    #include "input.h"
#else
    #include "server.h"
#endif

std::vector<std::shared_ptr<Object>> objects, objectsDynamic;
std::vector<GLfloat> vertices;
std::mutex mutObj, mutVert;
std::atomic<bool> gameActive = true;

void threadUpdateObj();

int main()
{
    #if !defined(DEDICATED_SERVER)
        GLFWwindow* window;

        if(SteamAPI_RestartAppIfNecessary(STEAM_APP_ID))
            return 1;

        if(!SteamAPI_Init()||!glfwInit())
        {
            SteamAPI_Shutdown();
            return 1;
        }

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(mode->width, mode->height, "Schwertspitze", NULL, NULL);

        if(window==NULL)
        {
            SteamAPI_Shutdown();
            glfwTerminate();
            return 1;
        }

        glfwMakeContextCurrent(window);

        glewExperimental = true;

        if(glewInit()!=GLEW_OK)
        {
            SteamAPI_Shutdown();
            glfwTerminate();
            return 1;
        }

        Input controls(window, "./data/config.cfg");

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        GLuint programGL;

        //Bind shaders to program
        {
            GLuint shaderVertex = glCreateShader(GL_VERTEX_SHADER);
            GLuint shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
            GLint success;
            std::string shader;
            std::ifstream shaderFile("./data/shaders/shader.vert");

            if(shaderFile.is_open())
            {
                std::stringstream stream;

                stream<<shaderFile.rdbuf();

                shader = stream.str();

                shaderFile.close();
            }
            else
            {
                SteamAPI_Shutdown();
                glfwTerminate();
                return 1;
            }

            const char* shaderPtr = shader.c_str();

            glShaderSource(shaderVertex, 1, &shaderPtr, NULL);

            glCompileShader(shaderVertex);

            glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &success);

            if(success==GL_FALSE)
            {
                SteamAPI_Shutdown();
                glfwTerminate();
                return 1;
            }

            shaderFile.open("./data/shaders/shader.frag", std::ios::in);
            
            if(shaderFile.is_open())
            {
                std::stringstream stream;

                stream<<shaderFile.rdbuf();

                shader = stream.str();

                shaderFile.close();
            }
            else
            {
                SteamAPI_Shutdown();
                glfwTerminate();
                return 1;
            }

            shaderPtr = shader.c_str();

            glShaderSource(shaderFragment, 1, &shaderPtr, NULL);

            glCompileShader(shaderFragment);

            glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &success);

            if(success==GL_FALSE)
            {
                SteamAPI_Shutdown();
                glfwTerminate();
                return 1;
            }

            programGL = glCreateProgram();

            glAttachShader(programGL, shaderVertex);
            glAttachShader(programGL, shaderFragment);

            glLinkProgram(programGL);

            glGetProgramiv(programGL, GL_LINK_STATUS, &success);

            if(success==GL_FALSE)
            {
                SteamAPI_Shutdown();
                glDeleteProgram(programGL);
                glfwTerminate();
                return 1;
            }

            glDetachShader(programGL, shaderVertex);
            glDetachShader(programGL, shaderFragment);

            glDeleteShader(shaderVertex);
            glDeleteShader(shaderFragment);
        }
        
        GLuint vao, vbo;

        glGenVertexArrays(1, &vao);
	    glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glUseProgram(programGL);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        std::shared_ptr<Player> player = std::make_shared<Player>(0, 0);
        objects.push_back(player);
        objectsDynamic.push_back(player);

        std::shared_ptr<Menu> menu = std::make_shared<Menu>();
        objects.push_back(menu);
        objectsDynamic.push_back(menu);

        std::vector<std::thread> threads;
        threads.emplace_back(std::thread(threadUpdateObj));

        bool menuClosed = false;
        char controlBit = '\0';
        glfwSetTime(0);

        while(gameActive&&glfwWindowShouldClose(window)==0)
        {
            controlBit = controls.fetch();
            
            if(menuClosed&&controlBit!='~')
            {
                player->input(controlBit);

                glfwSetTime(0);
            }
            else if(glfwGetTime()>=0.1) //Prevent inhuman menu navigation
            {
                switch(menu->input(controlBit))
                {
                    case '~':
                        gameActive = false;
                        break;
                    case 'p':
                        menuClosed = true;
                        break;
                    case 'f':
                        if(glfwGetWindowMonitor(window)==NULL)
                        {
                            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                        }
                        else
                        {
                            int xpos, ypos;
                            monitor = glfwGetWindowMonitor(window);
                            glfwGetMonitorPos(monitor, &xpos, &ypos);

                            glfwSetWindowMonitor(window, NULL, xpos, ypos, mode->width, mode->height, 0);
                        }
                        break;
                    default:
                        break;
                }

                glfwSetTime(0);
            }

            mutVert.lock();

            glClear(GL_COLOR_BUFFER_BIT);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
	        glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, vertices.size()/5);
            glBindVertexArray(0);

            mutVert.unlock();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        gameActive = false; //Handle possibility of loop ending due to window closing

        //Join threads
        for(auto& thread: threads)
            thread.join();

        //Free up resources
        SteamAPI_Shutdown();
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(programGL);
        glfwTerminate();
    #else
        //Construct server object within CLI
    #endif

    return 0;
}

void threadUpdateObj()
{
    while(gameActive)
    {
        //Prevent other threads from adding or removing from vector during iteration
        mutObj.lock();

        for(auto& obj: objectsDynamic)
        {
            obj->update();
        }

        //Prevent other threads from reading vector during assembly
        mutVert.lock();

        vertices.clear();

        for(auto& obj: objects)
        {
            std::vector<GLfloat> objVertices(obj->getVertices());
            
            vertices.insert(vertices.end(), objVertices.begin(), objVertices.end()); //append_range()
        }

        mutVert.unlock();
        mutObj.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
