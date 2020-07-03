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
	Sphere::Sphere(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		m_Pos = position;
		m_Scale = scale;
		m_Rotation = rotation;

		m_LastPos = m_Pos;
		m_LastScale = m_Scale;
		m_LastRotation = m_Rotation;
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
		m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLE_STRIP, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		m_VertexArray->Unbind();
	}

	glm::mat4 Sphere::GetModel()
	{
		glm::mat4 model = glm::mat4(1.0f);
		if (m_LastScale.x == 0)
			model = glm::scale(model, glm::vec3(m_Scale.x, 0, 0));
		else
			model = glm::scale(model, glm::vec3(m_Scale.x/m_LastScale.x, 0, 0));
		if (m_LastScale.y == 0)
			model = glm::scale(model, glm::vec3(0, m_Scale.y, 0));
		else
			model = glm::scale(model, glm::vec3(0, m_Scale.y/m_LastScale.y, 0));
		if (m_LastScale.z == 0)
			model = glm::scale(model, glm::vec3(0, 0, m_Scale.z));
		else
			model = glm::scale(model, glm::vec3(0, 0, m_Scale.z/m_LastScale.z));

		model = glm::rotate(model, glm::radians(m_Rotation.z - m_LastRotation.z), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(m_Rotation.x - m_LastRotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(m_Rotation.y - m_LastRotation.y), glm::vec3(0, 1, 0));
		
		model = glm::translate(model, glm::vec3(m_Pos.x - m_LastPos.x, m_Pos.y - m_LastPos.y, m_Pos.z - m_LastPos.z));

		return model;
	}

	//------------------------------------------------------------------------
	//天空盒
	//-----------------------------------------------------------------------
	void SkyBox::Init()
	{
		float vertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
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

		m_CubeMap = LoadCubeMap(m_Faces);
	}

	void SkyBox::Render()
	{
		m_VertexArray->Bind();
		glBindTextureUnit(GL_TEXTURE_CUBE_MAP, m_CubeMap);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		m_VertexArray->Unbind();
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

	//------------------------------------------------------------------------
	//立方体
	//-----------------------------------------------------------------------
	Cube::Cube(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		m_Position = position;
		m_Rotation = rotation;
		m_Scale = scale;

		m_LastPosition = m_Position;
		m_LastScale = m_Scale;
		m_LastRotation = m_Rotation;
	}

	void Cube::Init()
	{
		float vertices[] = {
			// positions		  //texcoords	//normals  
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
				
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,	0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,	0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	0.0f,  1.0f,  0.0f
		};

		m_VertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoords"},
			{ShaderDataType::Float3, "a_Normal"}
		};

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

	}

	void Cube::Render()
	{
		m_VertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glm::mat4& Cube::GetModel()
	{
		
		if (m_LastScale.x == 0)
			m_Model = glm::scale(m_Model, glm::vec3(m_Scale.x, 1.0, 1.0));
		else
			m_Model = glm::scale(m_Model, glm::vec3(m_Scale.x / m_LastScale.x, 1.0, 1.0));
		if (m_LastScale.y == 0)
			m_Model  = glm::scale(m_Model, glm::vec3(1.0, m_Scale.y, 1.0));
		else
			m_Model = glm::scale(m_Model, glm::vec3(1.0, m_Scale.y / m_LastScale.y, 1.0));
		if (m_LastScale.z == 0)
			m_Model = glm::scale(m_Model, glm::vec3(1.0, 1.0, m_Scale.z));
		else
			m_Model = glm::scale(m_Model, glm::vec3(1.0, 1.0, m_Scale.z / m_LastScale.z));

		m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z - m_LastRotation.z), glm::vec3(0, 0, 1));
		m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x - m_LastRotation.x), glm::vec3(1, 0, 0));
		m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y - m_LastRotation.y), glm::vec3(0, 1, 0));

		m_Model = glm::translate(m_Model, glm::vec3(m_Position.x - m_LastPosition.x, m_Position.y - m_LastPosition.y, m_Position.z - m_LastPosition.z));


		return m_Model;
	}
}