#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
}; 

struct Light {
   vec3 position;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
   float lightIntensity = 4.0;
   vec3 ambient = light.ambient * material.ambient;

   float dis = length(light.position - FragPos);
   float lis = lightIntensity / (dis * dis);

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * material.diffuse * lis;

   float specularStrength = 0.5;
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 hvec = normalize(lightDir + viewDir);
   float spec = pow(max(dot(hvec, Normal), 0.0), material.shininess);
   vec3 specular = light.specular * spec * material.specular * lis;

   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result, 1.0);
}