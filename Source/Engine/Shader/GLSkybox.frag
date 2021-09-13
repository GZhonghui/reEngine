#version 330 core

in vec3 textureCoords;

out vec4 FragColor;

uniform samplerCube skyboxTexture;

void main()
{   
    textureCoords.x =  textureCoords.x;
    textureCoords.y = -textureCoords.y;
    textureCoords.z = -textureCoords.z;

    FragColor = texture(skyboxTexture, textureCoords);
}