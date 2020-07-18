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

uniform sampler2D u_Albedo;
uniform vec3 u_Ambient;

uniform vec3 u_CameraPos;

//高光因数
uniform float u_Shininess;
uniform float u_Specular;

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

//菲涅尔
vec3 Fresnel(vec3 reflectColor, vec3 refractColor);

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

	//菲涅尔
	vec3 FScolor = Fresnel(SkyBoxReflect(), result);
	//result += SkyBoxReflect()*0.1;
	//result += SkyBoxRefract()*0.1;
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
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);

	//结合Color
	vec3 ambient = u_Ambient * 0.01 * texture(u_Albedo, v_TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff *  texture(u_Albedo, v_TexCoords).rgb;
	vec3 specular = light.specular * spec * u_Specular * texture(u_Albedo, v_TexCoords).rgb;

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
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);

	//衰减
	float distance = length(light.position - v_WorldPos);
	float atten = 1.0/(light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * distance * distance);
	
	//结合
	vec3 ambient = u_Ambient * 0.01 * texture(u_Albedo, v_TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(u_Albedo, v_TexCoords).rgb;
	vec3 specular = light.specular * spec * u_Specular * texture(u_Albedo, v_TexCoords).rgb;

	ambient *= atten;
	diffuse *= atten;
	specular *= atten;
	
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

vec3 Fresnel(vec3 reflectColor, vec3 refractColor)
{
	vec3 viewDir = normalize(v_WorldPos - u_CameraPos);
	vec3 normal = normalize(v_Normal);
	float F0 = 0.5;
	float FSchlick = max(0, F0 + (1 - F0) * pow((1-dot(viewDir, normal)),5));
	vec3 color = mix(refractColor, reflectColor, FSchlick);
	return color;
}