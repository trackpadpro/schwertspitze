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

void Player::input(const char& commandBit)
{
    mutObj.lock();
    
    switch(commandBit)
    {
        default:
            break;
    }

    mutObj.unlock();
}

Menu::Menu()
{
    state = mPlay;

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
            case mPlay:
                vertices = {
                    -0.9f, +0.8f, 0.0f, 0.5f, 0.5f,
                    -0.95f, +0.75f, 1.0f, 0.0f, 0.5f,
                    -0.9f, +0.7f, 0.0f, 0.5f, 0.0f
                };
                break;
            case mKeybinds:
                vertices = {
                    -0.9f, +0.75f, 0.0f, 0.5f, 0.5f,
                    -0.95f, +0.7f, 1.0f, 0.0f, 0.5f,
                    -0.9f, +0.65f, 0.0f, 0.5f, 0.0f
                };
                break;
            case mKeybindSelection:
                break;
            case mFullscreen:
                vertices = {
                    -0.9f, +0.7f, 0.0f, 0.5f, 0.5f,
                    -0.95f, +0.65f, 1.0f, 0.0f, 0.5f,
                    -0.9f, +0.6f, 0.0f, 0.5f, 0.0f
                };
                break;
            case mVolume:
                vertices = {
                    -0.9f, +0.65f, 0.0f, 0.5f, 0.5f,
                    -0.95f, +0.6f, 1.0f, 0.0f, 0.5f,
                    -0.9f, +0.55f, 0.0f, 0.5f, 0.0f
                };
                break;
            case mVolumeSlider:
                break;
            case mHidden:
                vertices = {};
                break;
            case mExit:
                vertices = {
                    -0.9f, +0.6f, 0.0f, 0.5f, 0.5f,
                    -0.95f, +0.55f, 1.0f, 0.0f, 0.5f,
                    -0.9f, +0.5f, 0.0f, 0.5f, 0.0f
                };
                break;
            case mExitConfirm:
                break;
            default:
                break;
        }

        mutObj.unlock();
    }
}

char Menu::input(const char& commandBit)
{
    char menuBit = '\0';

    mutObj.lock();

    menuOption prev = state;

    switch(commandBit)
    {
        case 'w':
            if(state%10==0&&state>0)
            {
                state = (menuOption)(state-10);
            }
            else if(state==mPlay)
            {
                state = mExit;
            }
            else if(state==mVolumeSlider)
            {
                //volume up
            }
            break;
        case 's':
            if(state%10==0&&state<40)
            {
                state = (menuOption)(state+10);
            }
            else if(state==mExit)
            {
                state = mPlay;
            }
            else if(state==mVolumeSlider)
            {
                //volume down
            }
            break;
        case '~':
            if(state%10==0&&state<99)
            {
                state = mHidden;
            }
            else if(state==mHidden)
            {
                state = mPlay;
            }
            else
            {
                state = (menuOption)(state-1);
            }
            break;
        case 'X':
            switch(state)
            {
                case mPlay:
                    state = mHidden;
                    menuBit = 'p';
                    break;
                case mKeybinds:
                    state = mKeybindSelection;
                    break;
                case mFullscreen:
                    menuBit = 'f';
                    break;
                case mVolume:
                    state = mVolumeSlider;
                    break;
                case mVolumeSlider:
                    state = mVolume;
                    break;
                case mExit:
                    state = mExitConfirm;
                    break;
                case mExitConfirm:
                    menuBit = '~';
                    break;
                default:
                    break;
            }
        default:
            break;
    }

    //Only update menu when state changes
    if(prev==state)
        needsUpdate = false;
    else
        needsUpdate = true;

    mutObj.unlock();
    
    return menuBit;
}
