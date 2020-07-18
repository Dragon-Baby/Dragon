#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Normal;

out vec3 v_WorldPos;
out vec3 v_Normal;
out vec2 v_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	v_TexCoords = a_TexCoords;
	v_Normal = vec3(transpose(inverse(u_Model)) * vec4(a_Normal, 0.0));
	v_WorldPos = vec3(u_Model * vec4(a_Position, 1.0));
	gl_Position = u_Projection * u_View * vec4(v_WorldPos, 1.0);
}

#type fragment
#version 330 core
layout (location = 0) out vec4 f_Color;

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec2 v_TexCoords;

uniform sampler2D u_Albedo;
uniform vec3 u_Ambient;

//平行光
struct DirLight
{
	vec3 direction;
	vec3 diffuse;
};

//点光源
struct PointLight
{
	vec3 position;
	vec3 diffuse;
	vec3 attenuation;
};

//光源
uniform DirLight u_DirLight;


uniform int u_PointLightNum;
#define MAX_POINT_LIGHT_NUM 10
uniform PointLight u_PointLights[MAX_POINT_LIGHT_NUM];


//函数定义
vec3 CalDirLight(DirLight dirLight);
vec3 CalPointLight(PointLight pointLight);

//gamma
const float gamma = 2.2;

void main()
{
	//计算平行光
	vec3 result = CalDirLight(u_DirLight);

	//计算点光源
	for(int i = 0; i < u_PointLightNum; i++)
	{
		result += CalPointLight(u_PointLights[i]);
	}

	f_Color = vec4(pow(result, vec3(1.0/gamma)), 1.0);
}

vec3 CalDirLight(DirLight dirLight)
{
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(-dirLight.direction);

	//环境光
	vec3 ambient = u_Ambient*0.01 * pow(texture(u_Albedo, v_TexCoords).rgb, vec3(gamma));
	
	//漫反射因数
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * dirLight.diffuse * pow(texture(u_Albedo, v_TexCoords).rgb, vec3(gamma));

	vec3 result = ambient + diffuse;

	return result;
}

vec3 CalPointLight(PointLight pointLight)
{
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(pointLight.position - v_WorldPos);

	//环境光
	vec3 ambient = u_Ambient*0.01 * pow(texture(u_Albedo, v_TexCoords).rgb, vec3(gamma));
	
	//漫反射因数
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff *  pointLight.diffuse * pow(texture(u_Albedo, v_TexCoords).rgb, vec3(gamma));

	//衰减
	float distance = length(pointLight.position - v_WorldPos);
	float attenuation = 1.0 / (pointLight.attenuation.x + pointLight.attenuation.y * distance + pointLight.attenuation.z * distance * distance);


	vec3 result = (ambient + diffuse) * attenuation;

	return result;
}
