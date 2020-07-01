#include "dgpch.h"
#include "GameObjects.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/std_image/stb_image.h"

#include "Dragon/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace Dragon
{
	//-----------------------------------------------------------------------
	//球体-------------------------------------------------------------------
	//-----------------------------------------------------------------------
	Sphere::Sphere(const float radius, const glm::vec3& position)
	{
		m_Radius = radius;
		m_Pos = position;
	}

	void Sphere::Init()
	{
		m_VertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;

		const unsigned int X_SEGEMENTS = 64;
		const unsigned int Y_SEGEMENTS = 64;
		const float PI = 3.14159265359;

		for (unsigned int y = 0; y <= Y_SEGEMENTS; y++)
		{
			for (unsigned int x = 0; x <= X_SEGEMENTS; x++)
			{
				float xSegement = (float)x / (float)X_SEGEMENTS;
				float ySegement = (float)y / (float)Y_SEGEMENTS;
				float xPos = std::cos(xSegement * 2.0f * PI) * std::sin(ySegement * PI);
				float yPos = std::cos(ySegement * PI);
				float zPos = std::cos(xSegement * 2.0f * PI) * std::sin(ySegement * PI);

				m_Positions.push_back(glm::vec3(xPos, yPos, zPos));
				m_UV.push_back(glm::vec2(xSegement, ySegement));
				m_Normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGEMENTS; y++)
		{
			if (!oddRow)
			{
				for (unsigned int x = 0; x <= X_SEGEMENTS; x++)
				{
					m_Indices.push_back(y * (X_SEGEMENTS + 1) + x);
					m_Indices.push_back((y + 1) * (X_SEGEMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGEMENTS; x>=0; x--)
				{
					m_Indices.push_back((y + 1) * (X_SEGEMENTS + 1) + x);
					m_Indices.push_back(y * (X_SEGEMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		std::vector<float> data;
		for (unsigned int i = 0; i < m_Positions.size(); i++)
		{
			data.push_back(m_Positions[i].x);
			data.push_back(m_Positions[i].y);
			data.push_back(m_Positions[i].z);
			if (m_UV.size() > 0)
			{
				data.push_back(m_UV[i].x);
				data.push_back(m_UV[i].y);
			}
			if (m_Normals.size() > 0)
			{
				data.push_back(m_Normals[i].x);
				data.push_back(m_Normals[i].y);
				data.push_back(m_Normals[i].z);
			}
		}

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoords"},
			{ShaderDataType::Float3, "a_Normals"}
		};

		vertexBuffer.reset(VertexBuffer::Create(&data[0], data.size() * sizeof(float)));
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		indexBuffer.reset(IndexBuffer::Create(&m_Indices[0], m_Indices.size()));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void Sphere::Render()
	{
		glDepthMask(GL_FALSE);
		Renderer::Submit(m_VertexArray);
	}

	//------------------------------------------------------------------------
	//天空盒
	//-----------------------------------------------------------------------
	void SkyBox::Init()
	{
		float vertices[3 * 8] = {
			1.0f, 1.0f, -1.0f,	//0
			1.0f, 1.0f, 1.0f,	//1
			1.0f, -1.0f, 1.0f,	//2
			-1.0f, -1.0f, -1.0f,	//3

			-1.0f, 1.0f, -1.0f,		//4
			-1.0f, 1.0f, 1.0f,	//5
			-1.0f, -1.0f, 1.0f,	//6
			1.0f, -1.0f, -1.0f	//7
		};

		unsigned int indices[3 * 12] = {
			0,1,2,
			0,2,3,	//右
			4,7,5,
			5,7,6,	//左
			0,4,5,
			0,5,7,	//上
			2,6,7,
			2,7,3,	//下
			1,5,6,
			1,6,2,	//前
			4,0,3,
			4,3,7	//后
		};

		m_VertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_CubeMap = LoadCubeMap(m_Faces);
	}

	void SkyBox::Render()
	{
		glDepthMask(GL_FALSE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);
		Renderer::Submit(m_VertexArray);
		glDepthMask(GL_TRUE);
	}

	unsigned int SkyBox::LoadCubeMap(std::vector<std::string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}
}