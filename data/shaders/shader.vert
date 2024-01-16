#version 330 core

layout (location = 0) in vec2 vertPos;  //[x, y]
layout (location = 1) in vec3 vertCol;  //[r, g, b]

out vec4 vertexColor;

uniform mat2 aspRat; //[scaleWidth, 0; 0, scaleHeight]

void main()
{
    gl_Position = vec4(aspRat*vertPos, 0, 1.0);
    vertexColor = vec4(vertCol.rgb, 1.0);
}
