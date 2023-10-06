#version 330 

#include ../../../Engine/Resources/Engine/Shaders/test.incl

// ----- Definitions ----- //

#define NR_POINT_LIGHTS 8 
#define NR_DIR_LIGHT 8 

// ----- In ----- //

in vec3 Color; 
in vec2 texCoord; 
in vec3 Normal; 
in vec3 FragPos; 
in vec4 FragPosInDirLightSpace;

// ----- Out ----- //

out vec4 colour; 

// ----- Structs ----- //

struct Material 
{ 
	// textures 
	sampler2D texture_diffuse; 
	sampler2D texture_specular; 
	sampler2D texture_normal;
	bool useNormal;
	float shininess; 
}; 

struct PointLight 
{ 
	vec4 position; 
	vec4 color; 
}; 

struct DirLight 
{ 
	vec4 direction; 
	vec4 color; 
}; 

// ----- Uniforms ----- //

uniform vec3 viewPos; 
uniform Material material; 

uniform sampler2D shadowMap;

layout (std140) uniform Lights
{
	int pointLightCount;
	int dirLightCount;
	PointLight pointLights[NR_POINT_LIGHTS];
	DirLight dirLight[NR_DIR_LIGHT]; 
};
 
// ----- Forward Declerations ----- //

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 aTexCoord);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 aTexCoord);
float shadowCalculations(vec4 fragPos);
 
// ----- Methods ----- //
 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 aTexCoord) 
{ 
	foo();
	float constant = 1.f;
	float linear = .35f;
	float quadratic = .44f;

	float lightAmbient = .2f;
	float lightDiffuse = .5f;
	float lightSpecular = 1.f;
	
	vec3 lightDir = normalize(light.position.rgb - fragPos); 
	
	// diffuse shading 
	float diff = max(dot(normal, lightDir), 0.0); 
	
	// specular shading 
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); 
	
	// attenuation 
	float distance = length(light.position.rgb - fragPos); 
	float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance)); 
	
	// combine results 
	vec3 ambient = lightAmbient * vec3(texture(material.texture_diffuse, aTexCoord)); 
	vec3 diffuse = lightDiffuse * diff * vec3(texture(material.texture_diffuse, aTexCoord)); 
	vec3 specular = lightSpecular * spec * vec3(texture(material.texture_specular, aTexCoord)); 
	ambient *= attenuation; 
	diffuse *= attenuation; 
	specular *= attenuation; 
	return (ambient + diffuse + specular) * light.color.rgb; 
} 
 
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 aTexCoord) 
{ 
	float lightAmbient = .2f;
	float lightDiffuse = .5f;
	float lightSpecular = 1.f;
	
	vec3 lightDir = normalize(-light.direction.rgb); 
	
	// diffuse shading 
	float diff = max(dot(normal, lightDir), 0.0); 
	
	// specular shading 
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); 
	
	// combine results 
	vec3 ambient = lightAmbient * vec3(texture(material.texture_diffuse, aTexCoord)); 
	vec3 diffuse = lightDiffuse * diff * vec3(texture(material.texture_diffuse, aTexCoord)); 
	vec3 specular = lightSpecular * spec * vec3(texture(material.texture_specular, aTexCoord)); 
	
	float shadow = shadowCalculations(FragPosInDirLightSpace);
	return (ambient + (1.0 - shadow) * (diffuse + specular)) * light.color.rgb; 
}

float shadowCalculations(vec4 fragPos)
{
	// perform perspective divide
    vec3 projCoords = fragPos.xyz / fragPos.w;
	
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	
	projCoords = projCoords * 0.5 + 0.5; 
	
	float borderBias =  max(texelSize.x, texelSize.y) * 2;
	
	if(projCoords.x >= 1.0 - borderBias || projCoords.x <= borderBias ||
		projCoords.y >= 1.0 - borderBias || projCoords.y <= borderBias ||
		projCoords.z >= 1.0 - borderBias || projCoords.z <= borderBias)
        return 0.0;
	
	float shadow = 0;
	float bias = 0.005;
	float currentDepth = projCoords.z;
	
	
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
		}
	}
	
	shadow /= 9.0;
	
	return shadow;
}
 
void main() 
{ 
	vec3 result; 
	
	vec3 norm;
	if(material.useNormal)
	{
		norm = texture(material.texture_normal, texCoord).rgb;
		norm = normalize(norm * 2.0 - 1.0); 
 
	}
	else
	{
		norm = normalize(Normal);
	}
	vec3 viewDir = normalize(viewPos - FragPos); 
	
	// Directional lighting 
	for (int i = 0; i < dirLightCount; i++) 
	{
		result += CalcDirLight(dirLight[i], norm, viewDir, texCoord); 
	}
	
	for (int i = 0; i < pointLightCount; i++) 
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, texCoord); 
	}
 
	colour = vec4(result,1.0); 
} 