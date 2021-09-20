#version 330 core

in vec2 thisUV;
in vec3 thisNormal;

out vec4 FragColor;

uniform vec3      diffuseColor;

uniform int       enableDiffuseTexture;
uniform sampler2D diffuseTexture;

uniform int       enableNormalTexture;
uniform sampler2D normalTexture;

uniform int       enableSpecularTexture;
uniform sampler2D specularTexture;

uniform vec3  lightDir;
uniform vec3  lightColor;
uniform float lightPower;

void main()
{   
    vec3 objColor = diffuseColor;

    if(enableDiffuseTexture != 0)
    {
        vec4 textureColor = texture(diffuseTexture, thisUV);
        objColor = objColor * textureColor.xyz;
    }

    vec3 fixedNormal = thisNormal;
    
    if(enableNormalTexture != 0)
    {

    }

    fixedNormal =  normalize(fixedNormal);
    vec3 fixedLightDir = -normalize(lightDir);

    const float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor * lightPower;

    float diff = max(dot(fixedNormal, fixedLightDir), 0.0);
    vec3 diffuse = diff * lightColor * lightPower;

    vec3 lightPart = ambient + diff;

    FragColor = vec4(objColor * lightPart, 1.0);
}