#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture_diffuse1;

uniform samplerCube pointShadowMap;
uniform vec3 pointLightPos;

uniform vec3 viewPos;

#define EPS 1e-3

// uniform float far_plane;

float ShadowCalculation(samplerCube shadowMap, float bias, vec3 lightPos, vec3 fragPos)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // Use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(shadowMap, fragToLight).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    // return closestDepth;
    closestDepth *= 100.0;
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // Now test for shadows
    float visibility = currentDepth -  bias > closestDepth ? 0.0: 1.0;

    return visibility;
}

vec3 CalcPointLight(vec3 lightPos, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffTex, vec3 specTex, samplerCube shadowMap)
{
    vec3 ambient = 0.3 * diffTex;

    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    // float attenuation = 10.0 / pow(length(lightPos - fragPos), 2.0);
    float attenuation = 1.0;
    vec3 diffuse = diff * attenuation * diffTex;

    vec3 hvec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(hvec, normal), 0.0), 64);
    vec3 specular = spec * attenuation * specTex;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    return (ambient + (diffuse + specular) * ShadowCalculation(shadowMap, bias, lightPos, fragPos)) * 0.5;
}

void main()
{
    vec3 diffTex = texture(texture_diffuse1, fs_in.TexCoords).rgb;

    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 result = vec3(0.0);
    result += CalcPointLight(pointLightPos, normal, fs_in.FragPos, viewDir, diffTex, vec3(0.3), pointShadowMap);
    FragColor = vec4(result, 1.0f);
}