#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   sampler2D emission;
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
   float lightIntensity = 5.0;
   vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

   float dis = length(light.position - FragPos);
   float lis = lightIntensity / (dis * dis);

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * lis;

   float specularStrength = 0.5;
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 hvec = normalize(lightDir + viewDir);
   float spec = pow(max(dot(hvec, Normal), 0.0), material.shininess);
   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * lis;

   vec3 result = ambient + diffuse + specular + texture(material.emission, TexCoords).rgb;
   FragColor = vec4(result, 1.0);
}