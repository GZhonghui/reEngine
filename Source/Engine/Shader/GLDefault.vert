#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec2 thisUV;
out vec3 thisNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    thisUV = aUV;
    thisNormal = mat3(transpose(inverse(M))) * aNormal;

    gl_Position = P * V * M * vec4(aPos, 1.0);
}