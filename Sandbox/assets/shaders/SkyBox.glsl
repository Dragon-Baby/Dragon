#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	v_TexCoords = a_Position;
	vec4 pos = u_Projection * u_View * vec4(a_Position, 1.0);
	gl_Position = pos.xyww;
}

#type fragment
#version 330 core

layout (location = 0) out vec4 FragColor;

in vec3 v_TexCoords;

uniform samplerCube u_SkyBox;

void main()
{
	FragColor = texture(u_SkyBox, v_TexCoords);
}