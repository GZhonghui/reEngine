#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 textureCoords;

uniform mat4 V;
uniform mat4 P;

void main()
{
    textureCoords = aPos;
    gl_Position = P * V * vec4(aPos, 1.0);
}