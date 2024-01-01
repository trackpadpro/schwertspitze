#include "object.h"

Object::Object()
{
    
}

Object::Object(const std::vector<GLfloat>& objVertices) //[x, y, r, g, b]*[3 vertices]*[# triangles]
{
    if(objVertices.size()%15!=0)
        return;
    
    vertices = objVertices;
}

std::vector<GLfloat> Object::getVertices()
{
    return vertices;
}

void Object::update()
{

}

Character::Character(const GLfloat& x, const GLfloat& y)
{
    vertices = {
        x-0.5f, y-0.5f, 0.0f, 0.5f, 0.5f,
        x+0.5f, y+0.5f, 1.0f, 0.0f, 0.5f,
        x-0.5f, y+0.5f, 0.0f, 0.5f, 0.0f
    };
}

void Character::update()
{
    vertices[6]*=0.95;
    
    vertices[7]*=0.98;
}
