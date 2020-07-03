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
		Sphere(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
		
		void Init();
		void Render();

		glm::vec3& GetPosition() { return m_Pos; }
		void SetPosition(glm::vec3 position) { m_LastPos = m_Pos; m_Pos = position; }

		glm::vec3& GetRotation() { return m_Rotation; }
		void SetRotation(glm::vec3 rotation) { m_LastRotation = m_Rotation; m_Rotation = rotation; }

		glm::vec3& GetScale() { return m_Scale; }
		void SetScale(glm::vec3 scale) { m_LastScale = m_Scale; m_Scale = scale; }

		glm::mat4 GetModel();

	private:
		glm::vec3 m_Pos;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::vec3 m_LastPos;
		glm::vec3 m_LastRotation;
		glm::vec3 m_LastScale;

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

		glm::mat4 GetView() { return m_View; }
		glm::mat4 GetProjection() { return m_Projection; }
	private:
		unsigned int LoadCubeMap(std::vector<std::string> faces);
	private:
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);
		std::vector<std::string> m_Faces;
		std::shared_ptr<VertexArray> m_VertexArray;
		unsigned int m_CubeMap;
	};

	class Cube
	{
	public:
		Cube(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));

		void Init();
		void Render();
		glm::vec3& GetPosition() { return m_Position; }
		void SetPosition(glm::vec3 position) { m_Position = position; }

		glm::vec3& GetRotation() { return m_Rotation; }
		void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }

		glm::vec3& GetScale() { return m_Scale; }
		void SetScale(glm::vec3 scale) { m_Scale = scale; }

		glm::mat4& GetModel();

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::vec3 m_LastPosition;
		glm::vec3 m_LastRotation;
		glm::vec3 m_LastScale;
		glm::mat4 m_Model = glm::mat4(1.0f);
		std::shared_ptr<VertexArray> m_VertexArray;		
	};

	class DirLight
	{
	public:
		DirLight(glm::vec3 direction = glm::vec3(-1.0, -1.0, 0.0), glm::vec3 diffuse = glm::vec3(1.0, 1.0, 1.0), glm::vec3 specular = glm::vec3(1.0, 1.0, 1.0))
			:m_Direction(direction), m_Diffuse(diffuse), m_Specular(specular)
		{

		}

		glm::vec3 GetDirection() { return m_Direction; }
		void SetDirection(glm::vec3 direction) { m_Direction = direction; }

		glm::vec3 GetDiffuse() { return m_Diffuse; }
		void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }

		glm::vec3 GetSpecular() { return m_Specular; }
		void SetSpecular(glm::vec3 specular) { m_Specular = specular; }
	private:
		std::shared_ptr<VertexArray> m_VertexArray;
		glm::vec3 m_Direction;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};

	class PointLight
	{
	public:
		PointLight(glm::vec3 position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 diffuse = glm::vec3(0.1, 0.1, 0.1), glm::vec3 specular = glm::vec3(1.0, 1.0, 1.0), glm::vec3 attenuation = glm::vec3(1.0f, 0.09f, 0.032f))
			:m_Position(position), m_Diffuse(diffuse), m_Specular(specular), m_Attenuation(attenuation)
		{

		}

		glm::vec3 GetPosition() { return m_Position; }
		void SetPosition(glm::vec3 position) { m_Position = position; }

		glm::vec3 GetDiffuse() { return m_Diffuse; }
		void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }

		glm::vec3 GetSpecular() { return m_Specular; }
		void SetSpecular(glm::vec3 specular) { m_Specular = specular; }

		glm::vec3 GetAttenuation() { return m_Attenuation; }
		void SetAttenuation(glm::vec3 attenuation) { m_Attenuation = attenuation; }

	private:
		std::shared_ptr<VertexArray> m_VertexArray;
		glm::vec3 m_Position;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		glm::vec3 m_Attenuation;
	};
}