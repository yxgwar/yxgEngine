#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out float gShadow;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 WorldPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform sampler2D texture_diffuse;

layout(binding = 7) uniform sampler2D shadowMap;
uniform vec3 lightPos;

#define EPS 1e-3

#define SamplerNum 64

const vec2 poissonDisk[64] = vec2[]
(
    vec2(-0.955471, -0.295898),
    vec2(-0.726539, -0.813501),
    vec2(-0.615238, 0.507675),
    vec2(-0.418836, 0.0430319),
    vec2(-0.348735, -0.70455),
    vec2(-0.145604, 0.451659),
    vec2(-0.0707728, 0.978118),
    vec2(0.0592972, 0.240089),
    vec2(0.204077, -0.680105),
    vec2(0.346171, -0.18482),
    vec2(0.358745, 0.586596),
    vec2(0.435224, -0.594867),
    vec2(0.567247, 0.21424),
    vec2(0.586322, 0.835536),
    vec2(0.64275, -0.00726309),
    vec2(0.687704, -0.398114),
    vec2(-0.980071, 0.177648),
    vec2(-0.903287, 0.646138),
    vec2(-0.810266, 0.319437),
    vec2(-0.752068, 0.884182),
    vec2(-0.6592, 0.0180066),
    vec2(-0.639209, 0.399792),
    vec2(-0.579455, -0.204657),
    vec2(-0.515885, -0.542009),
    vec2(-0.465438, 0.170812),
    vec2(-0.433912, 0.775292),
    vec2(-0.407056, -0.268837),
    vec2(-0.315226, 0.000732422),
    vec2(-0.262917, 0.413923),
    vec2(-0.209571, -0.382641),
    vec2(-0.144749, -0.986663),
    vec2(-0.0196848, -0.138768),
    vec2(0.028656, -0.486587),
    vec2(0.0874963, 0.1395),
    vec2(0.126683, 0.588214),
    vec2(0.20539, -0.0174875),
    vec2(0.264565, -0.474227),
    vec2(0.325572, 0.145024),
    vec2(0.363121, 0.615772),
    vec2(0.425886, -0.282113),
    vec2(0.481582, -0.833705),
    vec2(0.525986, -0.0268564),
    vec2(0.574297, 0.511002),
    vec2(0.609333, -0.409131),
    vec2(0.672292, -0.85873),
    vec2(0.729331, 0.184881),
    vec2(0.786218, 0.635182),
    vec2(0.804254, -0.124943),
    vec2(0.860012, -0.55855),
    vec2(0.893246, 0.295724),
    vec2(0.929533, 0.800287),
    vec2(-0.990692, -0.0694275),
    vec2(-0.940214, 0.296426),
    vec2(-0.918302, 0.768548),
    vec2(-0.867977, 0.410443),
    vec2(-0.853664, -0.508499),
    vec2(-0.797693, -0.196692),
    vec2(-0.726676, 0.014801),
    vec2(-0.699088, 0.440718),
    vec2(-0.634297, -0.0967445),
    vec2(-0.598559, -0.45851),
    vec2(-0.533067, 0.0787689),
    vec2(-0.49852, 0.636494),
    vec2(-0.467421, -0.340312)
);

// 生成随机旋转矩阵（基于片段位置和时间的哈希函数）
mat2 getRandomRotation(vec2 uv)
{
    // 哈希函数生成随机角度
    float random = fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453 + 0);
    float angle = 6.283185 * random; // 0 ~ 2π
    // 构造旋转矩阵
    return mat2(cos(angle), sin(angle), -sin(angle), cos(angle));
}

float PCF(float bias)
{
    float filterRadius = 0.0004;

    float visibility = 0.0;
    mat2 rot = getRandomRotation(FragPos.xy); // 随机旋转矩阵

    // perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    for (int i = 0; i < SamplerNum; i++)
    {
        vec2 rpoisson = rot * poissonDisk[i];
        // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
        float closestDepth = texture(shadowMap, projCoords.xy + rpoisson * filterRadius).r;
        visibility += (projCoords.z - bias > closestDepth + EPS) ? 0.0 : 1.0;
    }
    return visibility / SamplerNum;
}

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - WorldPos);
    float bias = max(0.005 * (1.0 - dot(gNormal, lightDir)), 0.0005);
    gShadow = PCF(bias);
    // And the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(texture_diffuse, TexCoords).rgb;
    // gAlbedoSpec.rgb = vec3(0.95);
    // Store specular intensity in gAlbedoSpec's alpha component
    // gAlbedoSpec.a = texture(texture_specular, TexCoords).r;
    gAlbedoSpec.a = 0.05;
}