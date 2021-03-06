#type vertex
#version 330 core

layout(location = 0)in vec3 a_Position;
layout(location = 1)in vec2 a_TexCoords;
layout(location = 2)in vec3 a_Normal;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;

out vec2 v_TexCoords;
out vec3 v_WorldPos;
out vec3 v_Normal;

void main()
{
	v_TexCoords = a_TexCoords;
	v_WorldPos = vec3(u_Model * vec4(a_Position, 1.0))
	v_Normal = mat3(u_Model) * a_Normal;
	gl_Position = u_Projection * u_View * vec4(v_WorldPos, 1.0);
}

#type fragment
#version 330 core

#define lightNum 4
layout(location = 0)out vec4 FragColor;

in vec2 v_TexCoords;
in vec3 v_WorldPos;
in vec3 v_Normal;

// material
uniform vec3 u_Albedo;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_AO;

// lights
uniform vec3 u_LightPositions[lightNum];
uniform vec3 u_LightColors[lightNum];

uniform vec3 u_CameraPos;

const float PI = 3.14159265359;


float DistributionGGX(vec3 N, vec3 H, float u_Roughness)
{
	float a = u_Roughness*u_Roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float u_Roughness)
{
	float r = u_Roughness + 1.0;
	float k = (r*r)/8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float u_Roughness)
{
	float NdotV = max(dot(N,V), 0.0);
	float NdotL = max(dot(N,L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, u_Roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, u_Roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	vec3 N = normalize(v_Normal);
	vec3 V = normalize(u_CameraPos - v_WorldPos);

	//金属流程
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, u_Albedo, u_Metallic);

	//反射方程
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < lightNum; i++)
	{
		// 计算每个光源的辐射率
		vec3 L = normalize(u_LightPositions[i] - v_WorldPos);
		vec3 H = normalize(V + L);
		float distance = length(u_LightPositions[i] - v_WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = u_LightColors[i] * attenuation;

		//BRDF
		float NDF = DistributionGGX(N, H, u_Roughness);
		float G = GeometrySmith(N, V, L, u_Roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		vec3 specular = nominator / denomanator;

		// 菲涅尔
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);

		Lo += (kD * u_Albedo / PI + specular) ** radiance * NdotL;
	}

	// 环境光
	vec3 ambient = vec3(0.03) * u_Albedo * u_AO;
	vec3 color = ambient + Lo;

	// HDR色调映射
	color = color / (color + vec3(1.0));

	// 伽马校正
	color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);
}