#version 460 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture_diffuse;

void main()
{
    FragColor = vec4(texture(texture_diffuse, fs_in.TexCoords).rgb, 1.0);
    BrightColor = vec4(vec3(0.0), 1.0);
}