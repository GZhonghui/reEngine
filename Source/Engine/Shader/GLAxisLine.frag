#version 330 core

in vec3 thisColor;

out vec4 FragColor;

void main()
{   
    FragColor = vec4(thisColor, 1.0);
}