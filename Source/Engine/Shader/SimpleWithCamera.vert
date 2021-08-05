#version 450

layout(binding=0) uniform UniformMVP
{
    mat4 model;
    mat4 view;
    mat4 project;
}transform;

layout(location=0) in vec3 inPosition;

layout(location=0) out vec3 fragColor;

void main(void)
{
    vec4 model_position=transform.model*vec4(inPosition,1.0);
    
    gl_Position=transform.project*transform.view*model_position;
    gl_Position.y=-gl_Position.y;

    fragColor=vec3(1.0,0.0,0.0);
}