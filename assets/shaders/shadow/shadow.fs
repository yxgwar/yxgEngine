#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586
#define NUM_SAMPLES 100
#define NUM_RINGS 10

highp float rand_1to1(highp float x )
{ 
  // -1 -1
  return fract(sin(x)*10000.0);
}

highp float rand_2to1(vec2 uv )
{ 
  // 0 - 1
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot(uv.xy, vec2(a, b)), sn = mod(dt, PI);
	return fract(sin(sn) * c);
}

vec2 poissonDisk[NUM_SAMPLES];

void poissonDiskSamples( const in vec2 randomSeed )
{
    float ANGLE_STEP = PI2 * float(NUM_RINGS) / float(NUM_SAMPLES);
    float INV_NUM_SAMPLES = 1.0 / float(NUM_SAMPLES);

    float angle = rand_2to1(randomSeed) * PI2;
    float radius = INV_NUM_SAMPLES;
    float radiusStep = radius;

    for( int i = 0; i < NUM_SAMPLES; i ++ ) {
        poissonDisk[i] = vec2(cos(angle), sin(angle)) * pow(radius, 0.75);
        radius += radiusStep;
        angle += ANGLE_STEP;
    }
}

float ShadowCalculation(vec3 projCoords, float bias)
{
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    
    float visibility = currentDepth - bias > closestDepth + EPS  ? 0.0 : 1.0;

    return visibility;
}

float PCF(vec3 projCoords, float bias)
{
    vec2 texelSize = 2.0 / textureSize(shadowMap, 0);
    poissonDiskSamples(projCoords.xy);
    float visibility = 0.0;
    for(int i = 0; i < NUM_SAMPLES; i++)
    {
        float depth = texture(shadowMap, projCoords.xy + poissonDisk[i] * texelSize).r;
        if(projCoords.z - bias < depth + EPS)
        visibility++;
    }
    return visibility / float(NUM_SAMPLES);
}

void main()
{
    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.6);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor * 0.3;

    // calculate shadow
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // perform perspective divide
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    float visibility = 0.0;
    if(projCoords.z > 1.0)
        visibility = 1.0;
    else
        // visibility = ShadowCalculation(projCoords, bias);
        visibility = PCF(projCoords, bias);
        // visibility = PCSS(projCoords, bias);
    vec3 lighting = (ambient + visibility * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}