#pragma once

#include "Dragon/Renderer/Shader.h"
#include "Dragon/Renderer/Camera.h"
#include "Build_in/GameObjects.h"
#include "Dragon/Renderer/Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include <glm/glm.hpp>

namespace Dragon
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformBool(const std::string& name, bool value);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		std::map<std::string, std::vector<std::string>>& GetShaderParameterName() { return m_ShaderParametersName; };
		std::map<std::string, std::map<std::string, std::any>>& CreateParameter();
		std::map<std::string, std::map<std::string, std::any>>& GetParameter() { return m_ParametersToCreate; }
		void SetParameter(std::string& gameObjectName, GameObjectLibrary& gameObjectLibrary, Camera& camera, DirLight& dirLight, std::vector<PointLight>& pointLights);
		void UploadAllUniformParameter();
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<unsigned int, std::string> PreProcess(const std::string& source);
		void SetShaderParameterName(const std::string& source);
		void Compile(const std::unordered_map<unsigned int, std::string>& shaderSources);
		void ShaderParameterMapInit();
	private:
		uint32_t m_RendererID;
		GameObjectLibrary m_GameObjectLibrary;
		std::string m_Name;
		std::map<std::string, std::vector<std::string>> m_ShaderParametersName;

		std::map<std::string, std::map<std::string, std::any>> m_ParametersToCreate;

		bool m_FirstCreated = false;
	};
}