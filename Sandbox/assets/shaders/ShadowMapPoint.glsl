#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;

uniform mat4 u_Model;

void main()
{
	gl_Position = u_Model * vec4(a_Postion, 1.0);
}

#type geometry
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 u_ShadowMatrices[6];

out vec4 g_WorldPos;

void main()
{
	for(int face = 0; face < 6; face++)
	{
		gl_Layer = face;
		for(int i = 0; i < 3; i++)
		{
			g_WorldPos = gl_in[i].gl_Position;
			gl_Position = u_ShadowMatrices[face] * g_WorldPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}

#type fragment
in vec4 g_WorldPos;

uniform vec3 u_LightPos;
uniform float u_FarPlane;

void main()
{
	// 获取片段和光源位置的距离
	float lightDistance = length(g_WorldPos.xyz - u_LightPos);

	// 和远平面的值相乘将距离映射到[0,1]之间
	lightDistance = lightDistance / u_FarPlane;

	// 将距离作为当前片段深度值
	gl_FragDepth = lightDistance;
}