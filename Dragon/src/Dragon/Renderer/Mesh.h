#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"

#include <vector>

namespace Dragon
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures);

		void Draw(Shader shader);
	
	private:
		void setupMesh();
	private:
		unsigned int VAO, VBO, EBO;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> texture;

	};
}