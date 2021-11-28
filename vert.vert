#version 450 core

layout(location=0) in vec4 pos;
layout(location=1) in vec4 color;


uniform mat4 mvp;

out vec4 vertColor;

void main(void)
{
    gl_Position = mvp * pos;
    vertColor = color;
}
