#include <fstream>
#include "input.h"

namespace input
{

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
                        keyring[escape].push_back(std::stoi(sensor));
                    else if(command=="attack")
                        keyring[attack].push_back(std::stoi(sensor));
                    else if(command=="up")
                        keyring[up].push_back(std::stoi(sensor));
                    else if(command=="left")
                        keyring[left].push_back(std::stoi(sensor));
                    else if(command=="down")
                        keyring[down].push_back(std::stoi(sensor));
                    else if(command=="right")
                        keyring[right].push_back(std::stoi(sensor));
                }
            }
        }

        config.close();
    }
}

command Input::fetch()
{
    command commandByte = command::none;
    
    for(auto& sensor: keyring[escape])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
            return escape; //Esc takes precedence
    for(auto& sensor: keyring[attack])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
        {
            commandByte = (command)(commandByte | attack);
            break;
        }
    for(auto& sensor: keyring[up])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
        {
            commandByte = (command)(commandByte | up);
            break;
        }
    for(auto& sensor: keyring[left])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
        {
            commandByte = (command)(commandByte | left);
            break;
        }
    for(auto& sensor: keyring[down])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
        {
            commandByte = commandByte & up ? (command)(commandByte - up) : (command)(commandByte | down);
            break;
        }
    for(auto& sensor: keyring[right])
        if(glfwGetKey(window, sensor)==GLFW_PRESS)
        {
            commandByte = commandByte & left ? (command)(commandByte - left) : (command)(commandByte | right);
            break;
        }

    return commandByte;
}

} //namespace input
