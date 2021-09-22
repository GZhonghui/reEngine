#version 330 core

in vec2 thisUV;
in vec3 thisNormal;
in mat3 thisTBN;
in vec3 thisLocation;

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

uniform vec3 viewLocation;

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
        fixedNormal = texture(normalTexture, thisUV).rgb * 2.0 - 1.0;
        fixedNormal = thisTBN * fixedNormal;
    }

    fixedNormal =  normalize(fixedNormal);

    vec3 specularColor = vec3(1.0);

    if(enableSpecularTexture != 0)
    {
        specularColor = texture(specularTexture, thisUV).rgb;
    }

    vec3 fixedLightDir = normalize(-lightDir);

    vec3 LL = lightColor * lightPower;

    const float ambientStrength  = 0.1;
    const float diffuseStrength  = 1.0;
    const float specularStrength = 0.5;

    vec3 ambientPart = ambientStrength * LL *objColor;
  
    float diff = max(dot(fixedNormal, fixedLightDir), 0.0);
    vec3  diffusePart = diffuseStrength * diff * LL * objColor;

    
    vec3  viewDir = normalize(viewLocation - thisLocation);
    vec3  reflectDir = reflect(-fixedLightDir, fixedNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3  specularPart = specularStrength * spec * LL * specularColor;

    vec3 Final = ambientPart + diffusePart + specularPart;

    FragColor = vec4(Final, 1.0);
}