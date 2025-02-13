#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat3 NormalM;

void main()
{
    vs_out.normal = NormalM * aNormal;
    gl_Position = model * vec4(aPos, 1.0); 
}