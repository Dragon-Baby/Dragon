#type vertex
#version 330 core
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

void main()
{
	gl_Position = vec4(a_Position.xy, 0.0, 1.0);
	v_TexCoords = a_TexCoords;
}

#type fragment
#version 330 core
layout (location = 0) out vec4 f_Color;

in vec2 v_TexCoords;

uniform sampler2D u_ScreenTexture;
uniform bool u_Opposition;
uniform float u_OppositionWeight;
uniform bool u_Gray;
uniform float u_GrayWeight;
uniform bool u_Kernel;
uniform float u_KernelWeight;
uniform bool u_Blur;
uniform float u_BlurWeight;
uniform bool u_EdgeDetection;
uniform float u_EdgeDetectionWeight;

//后期处理函数
//反相
vec3 Opposition(vec3 color);

//灰度显示
vec3 Gray(vec3 color);

//克尔效应
vec3 Kernel(sampler2D screenTexture, vec2 texCoords);

//模糊
vec3 Blur(sampler2D screenTexture, vec2 texCoords);

//边缘检测
vec3 EdgeDetection(sampler2D screenTexture, vec2 texCoords);

void main()
{
	vec3 screenColor = texture(u_ScreenTexture, v_TexCoords).rgb;
	f_Color = vec4(screenColor, 1.0);
	if(u_Opposition)
		f_Color *= vec4(Opposition(screenColor), 1.0);
	if(u_Gray)
		f_Color *= vec4(Gray(screenColor),1.0);
	if(u_Kernel)
		f_Color *= vec4(Kernel(u_ScreenTexture, v_TexCoords), 1.0);
	if(u_Blur)
		f_Color *= vec4(Blur(u_ScreenTexture, v_TexCoords), 1.0);
	if(u_EdgeDetection)
		f_Color *= vec4(EdgeDetection(u_ScreenTexture, v_TexCoords), 1.0);
}

vec3 Opposition(vec3 color)
{
	return vec3(1.0 - color)*u_OppositionWeight;
}

vec3 Gray(vec3 color)
{
	float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	return vec3(average, average, average)*u_GrayWeight;
}

vec3 Kernel(sampler2D screenTexture, vec2 texCoords)
{
	const float offset = 1.0 / 720.0;

	//八邻接
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);

	//kernel矩阵
	float kernel[9] = float[](
		-1,-1,-1,
		-1,9,-1,
		-1,-1,-1
	);

	//获得采样点坐标
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
	}

	vec3 color = vec3(0.0);
	//滤波
	for(int i = 0; i < 9; i++)
	{
		color += sampleTex[i] * kernel[i];
	}

	return color*u_KernelWeight;
}

vec3 Blur(sampler2D screenTexture, vec2 texCoords)
{
	const float offset = 1.0 / 720.0;

	//八邻接
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);

	//kernel矩阵
	float kernel[9] = float[](
		1.0/16, 2.0/16, 1.0/16,
		2.0/16, 4.0/16, 2.0/16,
		1.0/16, 2.0/16, 1.0/16
	);

	//获得采样点坐标
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
	}

	vec3 color = vec3(0.0);
	//滤波
	for(int i = 0; i < 9; i++)
	{
		color += sampleTex[i] * kernel[i];
	}

	return color*u_BlurWeight;
}

vec3 EdgeDetection(sampler2D screenTexture, vec2 texCoords)
{
	const float offset = 1.0 / 720.0;

	//八邻接
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);

	//kernel矩阵
	float kernel[9] = float[](
		1,1,1,
		1,-8,1,
		1,1,1
	);

	//获得采样点坐标
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
	}

	vec3 color = vec3(0.0);
	//滤波
	for(int i = 0; i < 9; i++)
	{
		color += sampleTex[i] * kernel[i];
	}

	return color*u_EdgeDetectionWeight;
}