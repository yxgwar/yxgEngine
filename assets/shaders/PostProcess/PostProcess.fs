#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2DMS screenTexture;
uniform int samples;
uniform vec2 textureSize;

void main()
{
    const float gamma = 2.2;
    vec3 col = vec3(0.0);
    ivec2 uv = ivec2(TexCoords * textureSize);
    for(int i = 0; i < samples; i++)
        col += texelFetch(screenTexture, uv, i).rgb;
    col /= float(samples);
    col = pow(col, vec3(1.0 / gamma));
    FragColor = vec4(col, 1.0);
} 