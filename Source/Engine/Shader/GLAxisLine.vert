#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 thisColor;

uniform float lineLength;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    thisColor = aColor;
    gl_Position = P * V * M * vec4(aPos * lineLength, 1.0);
}