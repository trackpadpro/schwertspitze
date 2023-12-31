#include <fstream>
#include <cctype>
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
    bool attack = false, up = false, left = false, down = false, right = false;

    glfwPollEvents();
    
    for(auto& sensor: keyring['0'])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
            return '~'; //Esc takes precedence

    for(auto& sensor: keyring['1'])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
            attack = true;

    for(auto& sensor: keyring['w'])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
            up = true;

    for(auto& sensor: keyring['a'])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
            left = true;

    for(auto& sensor: keyring['s'])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
            down = true;

    for(auto& sensor: keyring['d'])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
            right = true;

    if(up&&down)
    {
        up = false;
        down = false;
    }

    if(left&&right)
    {
        left = false;
        right = false;
    }

    if(left)
    {
        if(up)
            commandBit = 'q';
        else if(down)
            commandBit = 'z';
        else
            commandBit = 'a';
    }
    else if(right)
    {
        if(up)
            commandBit = 'e';
        else if(down)
            commandBit = 'c';
        else
            commandBit = 'd';
    }
    else if(up)
        commandBit = 'w';
    else if(down)
        commandBit = 's';
    else if(attack)
        commandBit = 'x';

    if(attack)
        return std::toupper(commandBit);

    return commandBit;
}
