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
        std::vector<std::shared_ptr<Object>> objects;

        glUseProgram(programGL);

        bool gameActive = true;

        while(gameActive&&glfwWindowShouldClose(window)==0)
        {
            switch(controls.fetch())
            {
                case '~':
                    gameActive = false;
                    break;
                default:
                    break;
            }

            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        glDeleteProgram(programGL);
        glfwTerminate();
    #else
        //Construct server object within CLI
    #endif

    return 0;
}
