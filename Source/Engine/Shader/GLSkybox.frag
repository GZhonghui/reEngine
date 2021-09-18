#version 330 core

in vec3 textureCoords;

out vec4 FragColor;

uniform samplerCube skyboxTexture;

void main()
{   
    FragColor = texture(skyboxTexture, textureCoords);
}