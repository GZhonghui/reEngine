#version 330 core

in vec3 thisPosition;
in vec3 thisNormal;

out vec4 FragColor;

uniform vec3 cameraPos;

uniform samplerCube skyboxTexture;

uniform float N;

void main()
{   
    float Ratio = 1.0 / N;

    vec3 I = normalize(thisPosition - cameraPos);
    vec3 R = refract(I, normalize(thisNormal), Ratio);

    FragColor = vec4(texture(skyboxTexture, R).rgb, 1.0);
}