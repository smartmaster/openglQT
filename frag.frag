#version 450 core

in vec4 vertColor;
out vec4 finalColor;

void main(void)
{
    //gl_FragColor = vertColor;
    finalColor = vertColor;
}
