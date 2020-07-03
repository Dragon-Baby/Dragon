#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Normals;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout (location = 0) out vec4 f_Color;

void main()
{
	f_Color = vec4(1.0,0.0,0.0,1.0);
}