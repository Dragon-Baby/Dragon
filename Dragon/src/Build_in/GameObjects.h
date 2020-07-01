#pragma once

#include "Dragon/Renderer/VertexArray.h"
#include "Dragon/Renderer/Buffer.h"
#include "Dragon/Renderer/Shader.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dragon
{
	class Sphere
	{
	public:
		Sphere(const float radius = 1.0f, const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));
		
		void Init();
		void Render();

		glm::vec3& GetPosition() { return m_Pos; }
		void SetPosition(glm::vec3 position) { m_Pos = position; }
	private:
		float m_Radius;
		glm::vec3 m_Pos;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::vector<glm::vec3> m_Positions;
		std::vector<glm::vec2> m_UV;
		std::vector<glm::vec3> m_Normals;
		std::vector<unsigned int> m_Indices;
	};

	class SkyBox
	{
	public:
		void Init();
		void Render();
		void SetView(glm::mat4 view) { m_View = view; }
		void SetProjection(glm::mat4 projection) { m_Projection = projection; }
		void SetFaces(std::vector<std::string> faces) { m_Faces = faces; }
		void SetShader(std::shared_ptr<Shader> shader) { m_Shader = shader; }

		glm::mat4 GetView() { return m_View; }
		glm::mat4 GetProjection() { return m_Projection; }
	private:
		unsigned int LoadCubeMap(std::vector<std::string> faces);
	private:
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);
		std::vector<std::string> m_Faces;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Shader> m_Shader;
		unsigned int m_CubeMap;
	};
}