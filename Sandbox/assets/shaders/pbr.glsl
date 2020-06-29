#type vertex
#version 330 core

layout(location = 0)in vec3 a_Position;
layout(location = 1)in vec2 a_TexCoord;
layout(location = 2)in vec3 a_Normal;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;

out vec2 v_TexCoord;
out vec3 v_WorldPos;
out vec3 v_Normal;

void main()
{
	v_TexCoord = a_TexCoord;
	v_WorldPos = vec3(u_Model * vec4(a_Position, 1.0))
	v_Normal = mat3(u_Model) * a_Normal;
	gl_Position = u_Projection * u_View * vec4(v_WorldPos, 1.0);
}

#type fragment
#version 330 core
#define lightNum 4
layout(location = 0)out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 v_WorldPos;
in vec3 v_Normal;

// material
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_MetallicMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_AOMap;

// lights
uniform vec3 u_LightPositions[lightNum];
uniform vec3 u_LightColors[lightNum];

uniform vec3 u_CameraPos;

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
	vec3 tangentNormal = texture(u_NormalMap, v_TexCoord).xyz *2.0 -1.0;

	vec3 Q1 = dFdx(v_WorldPos);
	vec3 Q2 = dFdy(v_WorldPos);
	vec2 st1 = dFdx(v_TexCoord);
	vec2 st2 = dFdy(v_TexCoord);

	vec3 N = normalize(v_Normal);
	vec3 T = normalize(Q1*st2.t - Q2*st2.t);
	vec3 B = -normalize(cross(N,T));
	mat3 TBN = mat3(T,B,N);

	return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r*r)/8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N,V), 0.0);
	float NdotL = max(dot(N,L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	vec3 albedo = pow(texture(u_AlbedoMap, v_TexCoord).rgb, vec3(2.2));
	float metallic = texture(u_MetallicMap, v_TexCoord).r;
	float roughness = texture(u_RoughnessMap, v_TexCoord).r;
	float ao = texture(u_AOMap, v_TexCoord).r;

	vec3 N = getNormalFromMap();
	vec3 V = normalize(u_CameraPos - v_WorldPos);

	//金属流程
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

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
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		vec3 specular = nominator / denomanator;

		// 菲涅尔
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);

		Lo += (kD * albedo / PI + specular) ** radiance * NdotL;
	}

	// 环境光
	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo;

	// HDR色调映射
	color = color / (color + vec3(1.0));

	// 伽马校正
	color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);
}