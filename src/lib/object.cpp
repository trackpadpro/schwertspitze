#include "object.h"

Object::Object(const std::vector<GLfloat>& objVertices) //[x, y, r, g, b]*[3 vertices]*[# triangles]
{
    if(objVertices.size()%15!=0)
        return;
    
    vertices = objVertices;
}
