#version 330 core

layout (location = 0) in vec2 vertPos;  //[x, y]
layout (location = 1) in vec3 vertCol;  //[r, g, b]

out vec4 vertexColor;

void main()
{
    vertexColor = vec4(vertCol.rgb, 1.0);
    gl_Position = vec4(vertPos.xy, 0, 1.0);
}
