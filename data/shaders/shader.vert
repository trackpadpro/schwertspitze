#version 330 core

layout (location = 0) in vec2 vertPos;
layout (location = 1) in vec3 vertCol;

out vec4 vertexColor;

void main()
{
    vertexColor = vec4(vertCol.rgb, 1.0);
    gl_Position = vec4(vertPos.xy, 0, 1.0);
}
