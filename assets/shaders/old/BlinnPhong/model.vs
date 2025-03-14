#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

layout (std140, binding = 0) uniform Camera
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform mat3 NormalM;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(NormalM * aNormal);
    TexCoords = aTexCoords;
}