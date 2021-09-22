#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 thisUV;
out vec3 thisNormal;
out mat3 thisTBN;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    thisUV = aUV;
    thisNormal = mat3(transpose(inverse(M))) * aNormal;

    vec3 T = normalize(vec3(M * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(M * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(M * vec4(aNormal,    0.0)));

    thisTBN = mat3(T, B, N);

    gl_Position = P * V * M * vec4(aPos, 1.0);
}