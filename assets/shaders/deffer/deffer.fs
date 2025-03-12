#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gShadow;
uniform sampler2D ssao;

uniform vec3 lightPos;
uniform vec3 lightColor;

vec3 CalcPointLight(vec3 normal, vec3 viewDir, vec3 diffTex, float specTex, vec3 FragPos, float shadow, float ambientOcclusion)
{
    vec3 ambient = 0.3 * diffTex;

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffTex;

    vec3 hvec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(hvec, normal), 0.0), 64);
    float specular = spec * specTex;

    return (ambient * ambientOcclusion + (diffuse + specular) * shadow) * lightColor;
}

void main()
{
    // Retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 diffTex = texture(gAlbedoSpec, TexCoords).rgb;
    float specTex = texture(gAlbedoSpec, TexCoords).a;
    float shadow = texture(gShadow, TexCoords).r;
    float ambientOcclusion = texture(ssao, TexCoords).r;

    vec3 viewDir = normalize(-FragPos);

    vec3 result = vec3(0.0);
    result += CalcPointLight(normal, viewDir, diffTex, specTex, FragPos, shadow, ambientOcclusion);

    const float gamma = 2.2;
    result = pow(result, vec3(1.0 / gamma));
    
    FragColor = vec4(result, 1.0);
}