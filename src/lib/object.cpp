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
    
    return vertices;
}

void Object::update()
{
    if(needsUpdate)
    {
        mutObj.lock();
        
        

        mutObj.unlock();
    }
}

Character::Character(const GLfloat& x, const GLfloat& y)
{
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
    
    switch(state)
    {
        case mPlay:
            switch(commandBit)
            {
                case 'w':
                    state = mExit;
                    break;
                case 's':
                    state = mKeybinds;
                    break;
                case '~':
                    state = mHidden;
                    break;
                case 'X':
                    state = mHidden;
                    menuBit = 'p';
                    break;
                default:
                    break;
            }
            break;
        case mKeybinds:
            switch(commandBit)
            {
                case 'w':
                    state = mPlay;
                    break;
                case 's':
                    state = mFullscreen;
                    break;
                case '~':
                    state = mHidden;
                    break;
                case 'X':
                    state = mKeybindSelection;
                    break;
                default:
                    break;
            }
            break;
        case mKeybindSelection:
            switch(commandBit)
            {
                //Select command to remap
                case 'w':
                    
                    break;
                case 's':
                    
                    break;
                case '~':
                    state = mKeybinds;
                    break;
                case 'X':
                    
                    break;
                default:
                    break;
            }
            break;
        case mFullscreen:
            switch(commandBit)
            {
                case 'w':
                    state = mKeybinds;
                    break;
                case 's':
                    state = mVolume;
                    break;
                case '~':
                    state = mHidden;
                    break;
                case 'X':
                    menuBit = 'f';
                    break;
                default:
                    break;
            }
            break;
        case mVolume:
            switch(commandBit)
            {
                case 'w':
                    state = mFullscreen;
                    break;
                case 's':
                    state = mExit;
                    break;
                case '~':
                    state = mHidden;
                    break;
                case 'X':
                    state = mVolumeSlider;
                    break;
                default:
                    break;
            }
            break;
        case mVolumeSlider:
            switch(commandBit)
            {
                case 'a':
                    //Decrease volume
                    break;
                case 'd':
                    //Increase volume
                    break;
                case '~':
                    state = mVolume;
                    break;
                case 'X':
                    state = mVolume;
                    break;
                default:
                    break;
            }
            break;
        case mExit:
            switch(commandBit)
            {
                case 'w':
                    state = mVolume;
                    break;
                case 's':
                    state = mPlay;
                    break;
                case '~':
                    state = mHidden;
                    break;
                case 'X':
                    state = mExitConfirm;
                    break;
                default:
                    break;
            }
            break;
        case mExitConfirm:
            switch(commandBit)
            {
                case '~':
                    state = mExit;
                    break;
                case 'X':
                    menuBit = '~';
                    break;
                default:
                    break;
            }
            break;
        case mHidden:
            switch(commandBit)
            {
                case '~':
                    state = mPlay;
                    break;
                default:
                    break;
            }
            break;
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
