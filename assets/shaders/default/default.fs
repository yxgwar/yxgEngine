#version 460 core
layout (location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture_diffuse;
uniform vec3 viewPos;

void main()
{
    FragColor = vec4(texture(texture_diffuse, fs_in.TexCoords).rgb, 1.0);
}