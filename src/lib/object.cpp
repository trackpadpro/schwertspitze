#include "object.h"

Object::Object()
{
    needsUpdate = false;
}

Object::Object(const std::vector<GLfloat>& objVertices) //[x, y, r, g, b]*[3 vertices]*[# triangles]
{
    if(objVertices.size()%15!=0)
        return;
    
    vertices = objVertices;
    needsUpdate = false;
}

std::vector<GLfloat> Object::getVertices()
{
    mutObj.lock();
    
    std::vector<GLfloat> objVertices = vertices;

    mutObj.unlock();
    
    return objVertices; //Target for future optimization
}

void Object::update()
{
    if(needsUpdate)
    {
        mutObj.lock();
        
        x+=dx;
        y+=dy;
        dx+=ddx;
        dy+=ddy;

        mutObj.unlock();
    }
}

Character::Character(const GLfloat& xpos, const GLfloat& ypos)
{
    x = xpos;
    y = ypos;
    
    vertices = {
        x-0.5f, y-0.5f, 0.0f, 0.5f, 0.5f,
        x+0.5f, y+0.5f, 1.0f, 0.0f, 0.5f,
        x-0.5f, y+0.5f, 0.0f, 0.5f, 0.0f
    };

    needsUpdate = true;
}

void Character::update()
{
    if(needsUpdate)
    {
        mutObj.lock();
        
        vertices[6]*=0.995;
        vertices[7]*=0.998;

        mutObj.unlock();
    }
}

void Player::input(const input::command& commandByte)
{
    mutObj.lock();
    
    switch(commandByte)
    {
        default:
            break;
    }

    mutObj.unlock();
}

namespace menu
{

Menu::Menu()
{
    state = play;

    vertices = {
        -0.9f, +0.8f, 0.0f, 0.5f, 0.5f,
        -0.95f, +0.75f, 1.0f, 0.0f, 0.5f,
        -0.9f, +0.7f, 0.0f, 0.5f, 0.0f
    };

    needsUpdate = false;
}

void Menu::update()
{
    if(needsUpdate)
    {
        mutObj.lock();
        
        switch(state)
        {
            case play:
                vertices = {
                    -0.9f, +0.8f, 0.0f, 0.5f, 0.5f,
                    -0.95f, +0.75f, 1.0f, 0.0f, 0.5f,
                    -0.9f, +0.7f, 0.0f, 0.5f, 0.0f
                };
                break;
            case keybinds:
                vertices = {
                    -0.9f, +0.75f, 0.0f, 0.5f, 0.5f,
                    -0.95f, +0.7f, 1.0f, 0.0f, 0.5f,
                    -0.9f, +0.65f, 0.0f, 0.5f, 0.0f
                };
                break;
            case keybindSelection:
                break;
            case fullscreen:
                vertices = {
                    -0.9f, +0.7f, 0.0f, 0.5f, 0.5f,
                    -0.95f, +0.65f, 1.0f, 0.0f, 0.5f,
                    -0.9f, +0.6f, 0.0f, 0.5f, 0.0f
                };
                break;
            case volume:
                vertices = {
                    -0.9f, +0.65f, 0.0f, 0.5f, 0.5f,
                    -0.95f, +0.6f, 1.0f, 0.0f, 0.5f,
                    -0.9f, +0.55f, 0.0f, 0.5f, 0.0f
                };
                break;
            case volumeSlider:
                break;
            case hidden:
                vertices = {};
                break;
            case exit:
                vertices = {
                    -0.9f, +0.6f, 0.0f, 0.5f, 0.5f,
                    -0.95f, +0.55f, 1.0f, 0.0f, 0.5f,
                    -0.9f, +0.5f, 0.0f, 0.5f, 0.0f
                };
                break;
            case exitConfirm:
                break;
            default:
                break;
        }
        needsUpdate = false;
        mutObj.unlock();
    }
}

menuOption Menu::input(const input::command& commandByte)
{
    mutObj.lock();

    menuOption prev = state;
    menuOption menuByte = hidden;

    switch(commandByte)
{
        case input::up:
        {
            if(state % 0x10 == 0 && state > play)
            {
                state = (menuOption)(state - 0x10);
            }
            else if(state == play)
            {
                state = exit;
            }
            else if(state == volumeSlider)
            {
                //volume up
            }
            break;
        }
        case input::down:
        {
            if(state % 0x10 == 0 && state < exit)
            {
                state = (menuOption)(state + 0x10);
            }
            else if(state == exit)
            {
                state = play;
            }
            else if(state == volumeSlider)
            {
                //volume down
            }
            break;
        }
        case input::escape:
        {
            if(state % 0x10 == 0 && state < hidden)
            {
                state = hidden;
            }
            else if(state == hidden)
            {
                state = play;
            }
            else
            {
                state = (menuOption)(state - 1);
            }
            break;
        }
        case input::attack:
        {
            switch(state)
            {
                case play:
                    state = hidden;
                    menuByte = play;
                    break;
                case keybinds:
                case volume:
                case exit:
                    state = (menuOption)(state + 1);
                    break;
                case fullscreen:
                    menuByte = fullscreen;
                    break;
                case volumeSlider:
                    state = volume;
                    break;
                case exitConfirm:
                    menuByte = exit;
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }

    //Only update menu when state changes
    needsUpdate = prev == state ? false : true;
    mutObj.unlock();
    return menuByte;
}

} //namespace menu
