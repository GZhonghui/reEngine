#version 330 core

in vec2 thisUV;

out vec4 FragColor;

uniform sampler2D diffuseTexture;

void main()
{   
    FragColor = texture(diffuseTexture, thisUV);
}