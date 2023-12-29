#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>
#include "steam_api.h"
#include "server.h"

#if !defined(DEDICATED_SERVER)
    #include "client.h"
    #include "input.h"
    #include "menu.h"
#else
    #include <iostream>
#endif

int main()
{
    #if !defined(DEDICATED_SERVER)
        glfwInit();

        GLFWwindow* window;
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(mode->width, mode->height, "Schwertspitze", NULL, NULL);
        glfwMakeContextCurrent(window);

        Input controls(window, "./data/config.cfg");
        
        bool gameActive = true;

        while(gameActive)
        {
            switch(controls.fetch())
            {
                case '~':
                    gameActive = false;
                    break;
                default:
                    break;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        glfwTerminate();
    #else
        //Construct server object within CLI
    #endif

    return 0;
}
