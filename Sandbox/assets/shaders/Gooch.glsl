#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Normal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec3 v_WorldNormal;
out vec3 v_WorldPos;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
	v_WorldPos = vec3(u_Model * vec4(a_Position, 1.0));
	v_WorldNormal = mat3(transpose(inverse(u_Model)))*a_Normal;
}

#type fragment
#version 330 core
layout (location = 0) out vec4 f_Color;

in vec3 v_WorldNormal;
in vec3 v_WorldPos;

uniform vec3 u_LightDir;
uniform vec3 u_CameraPos;

void main()
{
	vec3 normal = normalize(v_WorldNormal);
	vec3 lightDir = normalize(-u_LightDir);
	vec3 viewDir = normalize(u_CameraPos - v_WorldPos);
	vec3 cool = vec3(0,0,0.55) + 0.25 * vec3(0.8,0.2,0.3);
	vec3 warm = vec3(0.3,0.3,0)+ 0.25 * vec3(0.8,0.2,0.3);
	vec3 highlight = vec3(1,1,1);
	float t = (dot(normal, lightDir) + 1) / 2;
	vec3 reflectDir = reflect(-lightDir, normal);
	float s = clamp(100 * dot(reflectDir, viewDir) - 97, 0, 1);
	vec3 shaded = s * highlight + (1 - s)*(t * warm + (1 - t)*cool);
	f_Color = vec4(shaded, 1.0);
}