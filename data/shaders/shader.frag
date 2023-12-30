#version 330 core

out vec4 fragmentColor;
in vec4 vertexColor;

void main()
{
    fragmentColor = vertexColor;
}
