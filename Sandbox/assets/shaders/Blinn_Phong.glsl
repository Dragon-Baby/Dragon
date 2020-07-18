#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Normal;

out vec3 v_WorldPos;
out vec3 v_Normal;
out vec2 v_TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	v_WorldPos = vec3(u_Model * vec4(a_Position, 1.0));
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	v_TexCoords = a_TexCoords;
	gl_Position = u_Projection * u_View * vec4(v_WorldPos, 1.0);
}

#type fragment
#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec2 v_TexCoords;

uniform vec3 u_Albedo;
uniform vec3 u_Ambient;

uniform vec3 u_CameraPos;

//高光因数
uniform float u_Shininess;


struct DirLight
{
	vec3 direction;

	vec3 diffuse;
	vec3 specular;
};

uniform DirLight u_DirLight;

struct PointLight
{
	vec3 position;

	vec3 attenuation;

	vec3 diffuse;
	vec3 specular;
};

uniform int u_PointLightNum;
#define MAX_POINT_LIGHT_NUM 10
uniform PointLight u_PointLights[MAX_POINT_LIGHT_NUM];

//天空盒纹理
uniform samplerCube u_SkyBox;

//天空盒反射
vec3 SkyBoxReflect();
//天空盒折射
vec3 SkyBoxRefract();

vec3 CalDirLight(DirLight light);
vec3 CalPointLight(PointLight light);

void main()
{
	//平行光
	vec3 result = CalDirLight(u_DirLight);

	//点光源
	for(int i = 0; i < u_PointLightNum; i++)
	{
		result += CalPointLight(u_PointLights[i]);
	}

	FragColor = vec4(result, 1.0);
}


vec3 CalDirLight(DirLight light)
{
	//法线
	vec3 normal = normalize(v_Normal);
	
	//视角方向
	vec3 viewDir = normalize(u_CameraPos - v_WorldPos);
	
	//光源方向
	vec3 lightDir = normalize(-light.direction);

	//漫反射因数
	float diff = max(dot(normal, lightDir), 0.0);

	//高光因数
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfDir, normal), 0.0), u_Shininess);

	//结合Color
	vec3 ambient = u_Ambient * 0.1 * u_Albedo;
	vec3 diffuse = light.diffuse * diff *  u_Albedo;
	vec3 specular = light.specular * spec * u_Albedo;

	return (ambient + diffuse + specular);
}

vec3 CalPointLight(PointLight light)
{
	//法线
	vec3 normal = normalize(v_Normal);
	
	//视角方向
	vec3 viewDir = normalize(u_CameraPos - v_WorldPos);

	//光源方向
	vec3 lightDir = normalize(light.position - v_WorldPos);

	//漫反射因数
	float diff = max(dot(normal, lightDir), 0.0);

	//高光因数
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfDir, normal), 0.0), u_Shininess);

	//衰减
	float distance = length(light.position - v_WorldPos);
	float attenuation = 1.0/(light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * distance * distance);
	
	//结合
	vec3 ambient = u_Ambient * u_Albedo;
	vec3 diffuse = light.diffuse * diff * u_Albedo;
	vec3 specular = light.specular * spec * u_Albedo;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec3 SkyBoxReflect()
{
	vec3 viewDir = normalize(v_WorldPos - u_CameraPos);
	vec3 reflectDir = reflect(viewDir, normalize(v_Normal));
	return texture(u_SkyBox, reflectDir).rgb;
}

vec3 SkyBoxRefract()
{
	float ratio = 1.00 / 1.52;
	vec3 viewDir = normalize(v_WorldPos - u_CameraPos);
	vec3 refractDir = refract(viewDir, normalize(v_Normal), ratio);
	return texture(u_SkyBox, refractDir).rgb;
}