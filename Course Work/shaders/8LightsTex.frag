
#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 color;

#define NR_POINT_LIGHTS 8

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(u_ViewPos - FragPos);

	vec3 result;
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(u_PointLights[i], norm, FragPos, viewDir);

	color = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, TexCoords));
	ambient *= attenuation;
   	diffuse *= attenuation;
    specular *= attenuation;
	return (ambient + diffuse + specular);
}