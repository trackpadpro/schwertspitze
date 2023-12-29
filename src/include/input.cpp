#include <fstream>
#include "input.h"

Input::Input(GLFWwindow* summoner, const std::string& cfg)
{
    window = summoner;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    std::ifstream config(cfg);

    if(config.is_open()){
        while(config.good()){
            std::string temp;

            config>>temp;

            if(temp=="bind")
            {
                std::string controller, sensor, command;
                
                config>>controller>>sensor>>command;

                if(controller=="glfw")
                {
                    if(command=="escape")
                        keyring['0'].push_back(std::stoi(sensor));
                    else if(command=="attack")
                        keyring['1'].push_back(std::stoi(sensor));
                    else if(command=="up")
                        keyring['w'].push_back(std::stoi(sensor));
                    else if(command=="left")
                        keyring['a'].push_back(std::stoi(sensor));
                    else if(command=="down")
                        keyring['s'].push_back(std::stoi(sensor));
                    else if(command=="right")
                        keyring['d'].push_back(std::stoi(sensor));
                }
            }
        }

        config.close();
    }
}

char Input::fetch()
{
    char commandBit;

    glfwPollEvents();
    
    for(auto& sensor: keyring['0'])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
            commandBit = '~';

    return commandBit;
}
