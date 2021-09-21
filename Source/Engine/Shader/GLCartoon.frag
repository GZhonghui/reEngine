#version 330 core

in vec2 thisUV;
in vec3 thisNormal;

out vec4 FragColor;

uniform vec3 diffuseColor;
uniform vec3 lightDir;

void main()
{   
    vec3 objColor = diffuseColor;

    vec3 fixedNormal   = normalize( thisNormal);
    vec3 fixedLightDir = normalize(-lightDir);

    float Intensity = dot(fixedLightDir, fixedNormal);

    vec3 K = vec3(1.0);
    if(Intensity > 0.95)
        K = vec3(1.0);
    else if (Intensity > 0.5)
        K = vec3(0.6);
    else if(Intensity > 0.25)
        K = vec3(0.3);
    else
        K = vec3(0.1);

    FragColor = vec4(objColor * K, 1.0);
}