#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include "steam_api.h"
#include "object.h"
#include "server.h"

#if !defined(DEDICATED_SERVER)
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include "client.h"
    #include "input.h"
    #include "menu.h"
#else
    #include <iostream>
#endif

int main()
{
    #if !defined(DEDICATED_SERVER)
        GLFWwindow* window;
        
        if(!glfwInit())
            return -1;

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(mode->width, mode->height, "Schwertspitze", NULL, NULL);

        if(window==NULL)
        {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);

        glewExperimental = true;

        if(glewInit()!=GLEW_OK)
        {
            glfwTerminate();
            return -1;
        }

        Input controls(window, "./data/config.cfg");

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        GLuint programGL;

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
                glfwTerminate();
                return -1;
            }

            const char* shaderPtr = shader.c_str();

            glShaderSource(shaderVertex, 1, &shaderPtr, NULL);

            glCompileShader(shaderVertex);

            glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &success);

            if(success==GL_FALSE)
            {
                glfwTerminate();
                return -1;
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
                glfwTerminate();
                return -1;
            }

            shaderPtr = shader.c_str();

            glShaderSource(shaderFragment, 1, &shaderPtr, NULL);

            glCompileShader(shaderFragment);

            glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &success);

            if(success==GL_FALSE)
            {
                glfwTerminate();
                return -1;
            }

            programGL = glCreateProgram();

            glAttachShader(programGL, shaderVertex);
            glAttachShader(programGL, shaderFragment);

            glLinkProgram(programGL);

            glGetProgramiv(programGL, GL_LINK_STATUS, &success);

            if(success==GL_FALSE)
            {
                glDeleteProgram(programGL);
                glfwTerminate();
                return -1;
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

        std::vector<std::shared_ptr<Object>> objects;
        std::vector<GLfloat> vertices;
        bool gameActive = true;

        std::shared_ptr<Player> player = std::make_shared<Player>(0, 0);
        objects.push_back(player);

        while(gameActive&&glfwWindowShouldClose(window)==0)
        {
            switch(controls.fetch())
            {
                case '~':
                    gameActive = false;
                    break;
                case 'X':
                    objects.emplace_back(std::make_shared<Character>(-0.2, 0.2));
                default:
                    break;
            }

            vertices.clear();

            for(auto& obj: objects)
            {
                obj->update();
                
                std::vector<GLfloat> objVertices(obj->getVertices());
                
                vertices.insert(vertices.end(), objVertices.begin(), objVertices.end()); //append_range()
            }

            glClear(GL_COLOR_BUFFER_BIT);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
	        glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, vertices.size()/5);
            glBindVertexArray(0);

            glfwSwapBuffers(window);

            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(programGL);
        glfwTerminate();
    #else
        //Construct server object within CLI
    #endif

    return 0;
}
