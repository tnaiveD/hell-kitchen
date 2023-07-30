#version 330 core

out vec4 fColor;

in vec3 gColor;

void main()
{
    fColor = vec4(gColor, 1.0);   
}  