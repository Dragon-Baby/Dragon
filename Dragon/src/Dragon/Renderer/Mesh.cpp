#include "dgpch.h"

#include "Mesh.h"

#include "Renderer.h"

namespace Dragon
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures)
	{
		m_Vertex = vertices;
		m_Index = indices;
		m_Texture = textures;

		setupMesh();
	}

	void Mesh::Draw(std::shared_ptr<Shader>& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < m_Texture.size(); i++)
		{
			std::string number;
			std::string name = m_Texture[i]->GetType();
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specula")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

			std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt(name + number, i);

			m_Texture[i]->Bind();
		}
		
		m_VAO->Bind();
		Renderer::Submit(m_VAO);
		m_VAO->Unbind();
	}

	void Mesh::setupMesh()
	{
		m_VAO.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(&m_Vertex[0], m_Vertex.size() * sizeof(Vertex)));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float3, "a_Normal"}
		};

		vertexBuffer->SetLayout(layout);
		m_VAO->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(&m_Index[0], m_Index.size() * sizeof(unsigned int)));
		m_VAO->SetIndexBuffer(indexBuffer);

		m_VAO->Bind();

	}
}