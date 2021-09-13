#version 330 core

in vec2 thisUV;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform vec3      diffuseColor;

uniform vec3  lightDir;
uniform vec3  lightColor;
uniform float lightPower;

void main()
{   
    FragColor = texture(diffuseTexture, thisUV);
}