#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   // sampler2D emission;
   float shininess;
}; 

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
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight {
   vec3 position;
   vec3 direction;
   float cutOff;
   float outerCutOff;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;       
};
uniform SpotLight spotLight;

uniform Material material;
uniform vec3 viewPos;
uniform vec2 resolution;
uniform sampler2D lightTex;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffTex, vec3 specTex)
{
   vec3 lightDir = normalize(-light.direction);
   // 漫反射着色
   float diff = max(dot(normal, lightDir), 0.0);
   // 镜面光着色
   vec3 hvec = normalize(lightDir + viewDir);
   float spec = pow(max(dot(hvec, normal), 0.0), material.shininess);
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
   float spec = pow(max(dot(hvec, normal), 0.0), material.shininess);
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

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffTex, vec3 specTex, vec2 uv)
{
   vec3 lightDir = normalize(light.position - fragPos);
   // 漫反射着色
   float diff = max(dot(normal, lightDir), 0.0);
   // 镜面光着色
   vec3 hvec = normalize(lightDir + viewDir);
   float spec = pow(max(dot(hvec, normal), 0.0), material.shininess);
   // 衰减
   float distance    = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + 
               light.quadratic * (distance * distance));

   float theta = dot(lightDir, normalize(-light.direction)); 
   float epsilon = light.cutOff - light.outerCutOff;
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
   // 合并结果
   vec3 sky = vec3(texture(lightTex, uv));
   vec3 ambient  = light.ambient  * diffTex;
   vec3 diffuse  = light.diffuse  * diff * sky;
   vec3 specular = light.specular * spec * specTex;

   return (ambient + diffuse + specular) * attenuation * intensity;
}

void main()
{
   vec2 uv = (gl_FragCoord.xy - vec2((resolution.x - resolution.y) / 2.0, 0.0)) / resolution.yy;
   vec3 viewDir = normalize(viewPos - FragPos);

   vec3 diffTex = vec3(texture(material.diffuse, TexCoords));
   vec3 specTex = vec3(texture(material.specular, TexCoords));

   vec3 result = CalcDirLight(dirLight, Normal, viewDir, diffTex, specTex);
   for(int i = 0; i < NR_POINT_LIGHTS; i++)
      result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir, diffTex, specTex);
   result += CalcSpotLight(spotLight, Normal, FragPos, viewDir, diffTex, specTex, uv); 
   FragColor = vec4(result, 1.0);
}