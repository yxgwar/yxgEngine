#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

layout (std140, binding = 0) uniform Camera
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform mat3 NormalM;

void main()
{
    Normal = NormalM * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}