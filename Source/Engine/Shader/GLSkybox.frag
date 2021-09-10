#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube skybox;

void main()
{   
    vec3 rightTexCoords = vec3(TexCoords.xy, -TexCoords.z);
    FragColor = texture(skybox, rightTexCoords);
}