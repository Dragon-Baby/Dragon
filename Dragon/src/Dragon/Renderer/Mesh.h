#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Texture.h"

#include "VertexArray.h"
#include "Buffer.h"


#include <vector>

namespace Dragon
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec3 Normal;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);

		void Draw(std::shared_ptr<Shader>& shader);
	
	private:
		void setupMesh();
	private:
		std::shared_ptr<VertexArray> m_VAO;

		std::vector<Vertex> m_Vertex;
		std::vector<unsigned int> m_Index;
		std::vector<std::shared_ptr<Texture2D>> m_Texture;

	};
}