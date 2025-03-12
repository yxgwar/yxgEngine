#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 FragPos;
out vec3 WorldPos;
out vec2 TexCoords;
out vec3 Normal;
out vec4 FragPosLightSpace;

layout (std140, binding = 0) uniform Camera
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform mat3 NormalM;
uniform mat4 lightSpaceMatrix;

void main()
{
    vec4 worldp = model * vec4(position, 1.0);
    vec4 viewPos = view * worldp;
    FragPos = viewPos.xyz; 
    gl_Position = projection * viewPos;
    TexCoords = texCoords;
    
    Normal = NormalM * normal;
    FragPosLightSpace = lightSpaceMatrix * vec4(worldp.xyz, 1.0);
}