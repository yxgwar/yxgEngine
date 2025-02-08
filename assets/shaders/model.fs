#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct DirLight {
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};  
uniform DirLight dirLight;

struct PointLight {
   vec3 position;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};
uniform PointLight pointLights;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_height1;
uniform float shininess;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffTex, vec3 specTex)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 hvec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(hvec, normal), 0.0), shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * diffTex;
    vec3 diffuse  = light.diffuse  * diff * diffTex;
    vec3 specular = light.specular * spec * specTex;
    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffTex, vec3 specTex)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 hvec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(hvec, normal), 0.0), shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));
    // 合并结果
    vec3 ambient  = light.ambient  * diffTex;
    vec3 diffuse  = light.diffuse  * diff * diffTex;
    vec3 specular = light.specular * spec * specTex;
    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 diffTex = vec3(texture(texture_diffuse1, TexCoords));
    vec3 specTex = vec3(texture(texture_specular1, TexCoords));
    // vec3 normal = normalize(vec3(texture(texture_height1, TexCoords)));
    vec3 normal = normalize(Normal);

    vec3 result = CalcDirLight(dirLight, normal, viewDir, diffTex, specTex);
    result += CalcPointLight(pointLights, normal, FragPos, viewDir, diffTex, specTex);
    FragColor = vec4(result, 1.0);
}