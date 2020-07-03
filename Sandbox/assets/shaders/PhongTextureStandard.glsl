#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_TexCoords;
layout (location = 2) in vec3 a_Normal;

out vec3 v_WorldPos;
out vec3 v_Normal;
out vec2 v_TexCoords;
out vec4 v_LightSpacePos;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_LightSpaceMatrix;

void main()
{
	v_WorldPos = vec3(u_Model * vec4(a_Position, 1.0));
	v_Normal = a_Normal * inverse(mat3(model));
	v_TexCoords = a_TexCoords;
	v_LightSpacePos = u_LightSpaceMatrix * vec4(v_WorldPos, 1.0);
	gl_Position = u_Projection * u_View * vec4(v_WorldPos, 1.0);
}

#type fragment
#version 330 core
layout (location = 0) out FragColor;

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec2 v_TexCoords;
in vec4 v_LightSpacePos;

uniform vec3 viewPos;

//高光因数
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;
uniform float u_Shininess;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;

struct PointLight
{
	vec3 Position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define POINT_LIGHT_NUM 4
uniform PointLight pointLights[POINT_LIGHT_NUM]

//阴影贴图
uniform sampler2D u_ShadowMap;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalPointLight(PointLight light, vec3 normal, vec3 worldPos, vec3 viewDir);

void main()
{
	//法线
	vec3 normal = normalize(v_Normal);
	vec3 viewDir = normalize(viewPos - v_WorldPos);

	//平行光
	vec3 result = CalDirLight(dirLight, normal, viewDir);

	//点光源
	for(int i = 0; i < POINT_LIGHT_NUM; i++)
	{
		result += CalPointLight(pointLights[i], normal, v_WorldPos, viewPos);
	}

	FragColor = vec4(result, 1.0);
}


vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//光源方向
	vec3 lightDir = normalize(-light.direction);

	//漫反射因数
	float diff = max(dot(normal, lightDir), 0.0);

	//高光因数
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);

	//结合
	vec3 ambient = light.ambient * vec3(texture(u_DiffuseMap, v_TexCoords));
	vec3 diffuse = light.diffuse * diff *  vec3(texture(u_DiffuseMap, v_TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(u_SpecularMap, v_TexCoords));

	float shadow = ShadowCalculation(v_LightSpacePos);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * vec3(texture(u_DiffuseMap, v_TexCoords));

	return lighting;
}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 worldPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - worldPos);

	//漫反射因数
	float diff = max(dot(normal, lightDir), 0.0);

	//高光因数
	vec3 refelctDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);

	//衰减
	float distance = length(light.position - worldPos);
	float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * distance * distance);
	
	//结合
	vec3 ambient = light.ambient * vec3(texture(u_DiffuseMap, v_TexCoords));
	vec3 diffuse = light.diffuse * diff *  vec3(texture(u_DiffuseMap, v_TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(u_SpecularMap, v_TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	float shadow = ShadowCalculation(v_LightSpacePos);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * vec3(texture(u_DiffuseMap, v_TexCoords));

	return lighting;
}