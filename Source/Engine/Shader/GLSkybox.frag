#version 330 core

in vec3 textureCoords;

out vec4 FragColor;

uniform samplerCube skyboxTexture;

void main()
{   
    vec3 fixedTextureCoords;
    fixedTextureCoords.x =  textureCoords.x;
    fixedTextureCoords.y = -textureCoords.y;
    fixedTextureCoords.z = -textureCoords.z;

    FragColor = texture(skyboxTexture, fixedTextureCoords);
}