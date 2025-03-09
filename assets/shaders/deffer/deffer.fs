#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

vec3 CalcPointLight(vec3 normal, vec3 viewDir, vec3 diffTex, float specTex, vec3 FragPos)
{
    vec3 ambient = 0.3 * diffTex;

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffTex;

    vec3 hvec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(hvec, normal), 0.0), 64);
    float specular = spec * specTex;

    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
    return (ambient + diffuse + specular) * lightColor;
}

void main()
{
    // Retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 diffTex = texture(gAlbedoSpec, TexCoords).rgb;
    float specTex = texture(gAlbedoSpec, TexCoords).a;

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);
    result += CalcPointLight(normal, viewDir, diffTex, specTex, FragPos);
    
    FragColor = vec4(result, 1.0);
}