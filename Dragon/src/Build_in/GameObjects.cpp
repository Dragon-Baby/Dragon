#include "dgpch.h"
#include "GameObjects.h"

#include <glad/glad.h>


namespace Dragon
{
	//-----------------------------------------------------------------------
	//球体-------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void Sphere::Init()
	{
		// data init
		std::vector<glm::vec3> m_Positions;
		std::vector<glm::vec2> m_UV;
		std::vector<glm::vec3> m_Normals;
		std::vector<unsigned int> m_Indices;

		std::shared_ptr<VertexArray> vertexArray;
		SetVertexArray(vertexArray);
		GetVertexArray().reset(VertexArray::Create());
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
		GetVertexArray()->AddVertexBuffer(vertexBuffer);

		indexBuffer.reset(IndexBuffer::Create(&m_Indices[0], m_Indices.size()));
		GetVertexArray()->SetIndexBuffer(indexBuffer);
	}

	void Sphere::Render()
	{
		GetVertexArray()->Bind();
		glDrawElements(GL_TRIANGLE_STRIP, GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		GetVertexArray()->Unbind();
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
		m_CubeMap = TextureCube::Create(m_Faces);
	}

	void SkyBox::Render()
	{
		m_VertexArray->Bind();
		m_CubeMap->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		m_VertexArray->Unbind();
	}

	//------------------------------------------------------------------------
	//立方体
	//-----------------------------------------------------------------------
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
		std::shared_ptr<VertexArray> vertexArray;
		SetVertexArray(vertexArray);
		GetVertexArray().reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoords"},
			{ShaderDataType::Float3, "a_Normal"}
		};

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(layout);
		GetVertexArray()->AddVertexBuffer(vertexBuffer);

	}

	void Cube::Render()
	{
		GetVertexArray()->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		GetVertexArray()->Unbind();
	}

	//------------------------------------------------------------------------
	//屏幕四边形
	//-----------------------------------------------------------------------
	void ScreenQuad::Init()
	{
		float vertices[] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		m_VertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ShaderDataType::Float2, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoords"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
	}

	void ScreenQuad::Render()
	{
		m_VertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_VertexArray->Unbind();
	}

	//------------------------------------------------------------------------
	//平面
	//-----------------------------------------------------------------------
	void Quad::Init()
	{
		float vertices[] = {
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f
		};

		std::shared_ptr<VertexArray> vertexArray;
		SetVertexArray(vertexArray);
		GetVertexArray().reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoords"},
			{ShaderDataType::Float3, "a_Normal"}
		};

		vertexBuffer->SetLayout(layout);

		GetVertexArray()->AddVertexBuffer(vertexBuffer);
	}

	void Quad::Render()
	{
		GetVertexArray()->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		GetVertexArray()->Unbind();
	}

	void GameObjectLibrary::Add(const std::string& name, const std::shared_ptr<GameObject>& geometry)
	{
		DG_CORE_ASSERT(!Exists(name), "Geometry already exists!");
		geometry->SetName(name);
		m_Geometrys[name] = geometry;
	}

	void GameObjectLibrary::Add(const std::shared_ptr<GameObject>& geometry)
	{
		auto& name = geometry->GetName();
		Add(name, geometry);
	}

	std::shared_ptr<GameObject> GameObjectLibrary::Get(const std::string& name)
	{
		DG_CORE_ASSERT(Exists(name), "Geometry not found!");
		return m_Geometrys[name];
	}

	bool GameObjectLibrary::Exists(const std::string& name) const
	{
		return m_Geometrys.find(name) != m_Geometrys.end();
	}
}