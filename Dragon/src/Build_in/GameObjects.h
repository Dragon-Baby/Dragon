#pragma once

#include "Dragon/Renderer/VertexArray.h"

#include "Dragon/Renderer/Texture.h"

#include "Dragon/Renderer/Shader.h"

#include "../ImGui/imgui.h"

#include <vector>

#include <variant>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dragon
{
	class GameObject
	{
	public:
		GameObject(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f))
		{
			m_Position = position;
			m_Rotation = rotation;
			m_Scale = scale;

			m_LastPosition = m_Position;
			m_LastScale = m_Scale;
			m_LastRotation = m_Rotation;
		};
		virtual void Init() = 0;
		virtual void Render()= 0;
		//virtual void OnImGuiRender() = 0;

		// Transform
		glm::vec3& GetPosition() { return m_Position; }
		void SetPosition(glm::vec3 position) { m_LastPosition = m_Position; m_Position = position;  }

		glm::vec3& GetRotation() { return m_Rotation; }
		void SetRotation(glm::vec3 rotation) { m_LastRotation = m_Rotation; m_Rotation = rotation; }

		glm::vec3& GetScale() { return m_Scale; }
		void SetScale(glm::vec3 scale) { m_LastScale = m_Scale; m_Scale = scale; }

		glm::mat4& GetModel() {
			m_Model = glm::scale(m_Model, glm::vec3(glm::max(m_Scale.x, 0.01f)/glm::max(m_LastScale.x, 0.01f), glm::max(m_Scale.y, 0.01f)/ glm::max(m_LastScale.y, 0.01f), glm::max(m_Scale.z, 0.01f)/ glm::max(m_LastScale.z, 0.01f)));

			m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z - m_LastRotation.z), glm::vec3(0, 0, 1));
			m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x - m_LastRotation.x), glm::vec3(1, 0, 0));
			m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y - m_LastRotation.y), glm::vec3(0, 1, 0));

			m_Model = glm::translate(m_Model, glm::vec3(m_Position.x - m_LastPosition.x, m_Position.y - m_LastPosition.y, m_Position.z - m_LastPosition.z));


			return m_Model;
		};

		// Rendering
		std::shared_ptr<VertexArray>& GetVertexArray() { return m_VertexArray; }
		void SetVertexArray(std::shared_ptr<VertexArray> vertexArray) { m_VertexArray = vertexArray; }

		// Name
		std::string& GetName() { return m_Name; }
		void SetName(std::string name) { m_Name = name; }

		// Type
		std::string& GetType() { return m_Type; }
		void SetType(std::string type) { m_Type = type; }

		// Shader
		std::shared_ptr<Shader>& GetShader() { return m_Shader; }
		void SetShader(std::shared_ptr<Shader> shader) { m_Shader = shader; }

	private:
		// Transform
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::vec3 m_LastPosition;
		glm::vec3 m_LastRotation;
		glm::vec3 m_LastScale;
		glm::mat4 m_Model = glm::mat4(1.0f);

		// Shader
		std::shared_ptr<Shader> m_Shader;

		// Name
		std::string m_Name;

		// Type
		std::string m_Type;

		// Rendering
		std::shared_ptr<VertexArray> m_VertexArray;
	};

	class Sphere : public GameObject
	{
	public:
		Sphere() :GameObject() {};
		
		virtual void Init() override;
		virtual void Render() override;
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

		std::shared_ptr<TextureCube> GetCubeMap() { return m_CubeMap; }

	private:
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);
		std::vector<std::string> m_Faces;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<TextureCube> m_CubeMap;
	};

	class Cube : public GameObject
	{
	public:
		Cube() :GameObject() {};

		virtual void Init() override;
		virtual void Render() override;
	};

	class ScreenQuad
	{
	public:
		ScreenQuad() {}
		void Init();
		void Render();

	private:
		std::shared_ptr<VertexArray> m_VertexArray;
	};

	class Quad : public GameObject
	{
	public:
		Quad() :GameObject() {};
		virtual void Init() override;
		virtual void Render() override;
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

		glm::mat4 GetLightSpaceMatrix() 
		{ 
			return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f) * 
				glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
	private:
		std::shared_ptr<VertexArray> m_VertexArray;
		glm::vec3 m_Direction;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};

	class PointLight
	{
	public:
		PointLight(glm::vec3 position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 diffuse = glm::vec3(1.0, 1.0, 1.0), glm::vec3 specular = glm::vec3(1.0, 1.0, 1.0), glm::vec3 attenuation = glm::vec3(1.0f, 0.09f, 0.032f))
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

	class GameObjectLibrary
	{
	public:
		void Add(const std::string& name, const std::shared_ptr<GameObject>& geometry);
		void Add(const std::shared_ptr<GameObject>& geometry);

		std::shared_ptr<GameObject> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, std::shared_ptr<GameObject>> m_Geometrys;
	};
}