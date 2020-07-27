#include "dgpch.h"
#include "OpenGLShader.h"

#include "Build_in/GameObjects.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Dragon
{
	static unsigned int ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		DG_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		ShaderParameterMapInit();
		SetShaderParameterName(source);
		CreateParameter();
		Compile(shaderSources);

		//»ñÈ¡Ãû×Ö
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		std::unordered_map<unsigned int, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			DG_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	std::unordered_map<unsigned int, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<unsigned int, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			DG_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			DG_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::SetShaderParameterName(const std::string& source)
	{
		const char* uniformToken = "uniform";
		size_t uniformTokenLength = strlen(uniformToken);
		size_t pos = source.find(uniformToken, 0);
		while (pos != std::string::npos)
		{
			size_t semicolon = source.find_first_of(";", pos);
			DG_CORE_ASSERT(semicolon != std::string::npos, "Syntax error");
			size_t begin = pos + uniformTokenLength + 1;
			size_t middleBackspace = source.find_first_of(" ", begin);
			std::string type = source.substr(begin, middleBackspace - begin);
			if (type.find(" ") != std::string::npos)
				DG_CORE_ERROR("There's blank in {0}", type);
			std::string name = source.substr(middleBackspace+1, semicolon - middleBackspace-1);
			if (name.find(" ") != std::string::npos)
				DG_CORE_ERROR("There's blank in {0}", name);
			if (name.find_first_of("[",middleBackspace+1) != std::string::npos)
			{
				size_t bracket = name.find_first_of("[",middleBackspace+1);
				name = name.substr(middleBackspace+1, bracket);
			}
			m_ShaderParametersName[type].push_back(name);
			size_t eol = source.find_first_of("\r\n", semicolon);
			DG_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(uniformToken, nextLinePos);
		}
	}

	std::map<std::string, std::map<std::string, std::any>>& OpenGLShader::CreateParameter()
	{
		std::map<std::string, std::any> init = {};
		m_ParametersToCreate["float"] = init;
		m_ParametersToCreate["double"] = init;
		m_ParametersToCreate["int"] = init;
		m_ParametersToCreate["bool"] = init;
		m_ParametersToCreate["vec2"] = init;
		m_ParametersToCreate["vec3"] = init;
		m_ParametersToCreate["vec4"] = init;
		m_ParametersToCreate["mat2"] = init;
		m_ParametersToCreate["mat3"] = init;
		m_ParametersToCreate["mat4"] = init;
		m_ParametersToCreate["sampler2D"] = init;
		m_ParametersToCreate["samplerCube"] = init;
		m_ParametersToCreate["DirLight"] = init;
		m_ParametersToCreate["PointLight"] = init;
		int textureID = 0;
		for (auto kv : m_ShaderParametersName)
		{
			if (kv.first == "float")
			{
				for (auto v : kv.second)
				{
					float parameter = 0.0f;
					m_ParametersToCreate["float"][v].emplace<float>(parameter);
				}

			}
			else if (kv.first == "double")
			{
				for (auto v : kv.second)
				{
					double parameter = 0.0;
					m_ParametersToCreate["double"][v].emplace<double>(parameter);
				}
			}
			else if (kv.first == "int")
			{
				for (auto v : kv.second)
				{
					int parameter = 0;
					m_ParametersToCreate["int"][v].emplace<int>(parameter);
				}
			}
			else if (kv.first == "bool")
			{
				for (auto v : kv.second)
				{
					bool parameter = 0;
					m_ParametersToCreate["bool"][v].emplace<bool>(parameter);
				}
			}
			else if (kv.first == "vec2")
			{
				for (auto v : kv.second)
				{
					glm::vec2 parameter = glm::vec2(0.0f);
					m_ParametersToCreate["vec2"][v].emplace<glm::vec2>(parameter);
				}
			}
			else if (kv.first == "vec3")
			{
				for (auto v : kv.second)
				{
					glm::vec3 parameter = glm::vec3(0.0f);
					m_ParametersToCreate["vec3"][v].emplace<glm::vec3>(parameter);
				}
			}
			else if (kv.first == "vec4")
			{
				for (auto v : kv.second)
				{
					glm::vec4 parameter = glm::vec4(0.0f);
					m_ParametersToCreate["vec4"][v].emplace<glm::vec4>(parameter);
				}
			}
			else if (kv.first == "mat2")
			{
				for (auto v : kv.second)
				{
					glm::mat2 parameter = glm::mat2(0.0f);
					m_ParametersToCreate["mat2"][v].emplace<glm::mat2>(parameter);
				}
			}
			else if (kv.first == "mat3")
			{
				for (auto v : kv.second)
				{
					glm::mat3 parameter = glm::mat3(0.0f);
					m_ParametersToCreate["mat3"][v].emplace<glm::mat3>(parameter);
				}
			}
			else if (kv.first == "mat4")
			{
				for (auto v : kv.second)
				{
					glm::mat4 parameter = glm::mat4(1.0f);
					m_ParametersToCreate["mat4"][v].emplace<glm::mat4>(parameter);
				}
			}
			else if (kv.first == "sampler2D")
			{
				for (auto v : kv.second)
				{
					m_ParametersToCreate["sampler2D"][v].emplace<int>(textureID);
					textureID++;
				}
			}
			else if (kv.first == "samplerCube")
			{
				for (auto v : kv.second)
				{
					m_ParametersToCreate["samplerCube"][v].emplace<int>(textureID);
					textureID++;
				}
			}
			else if (kv.first == "DirLight")
			{
				for (auto v : kv.second)
				{
					std::shared_ptr<DirLight> parameter;
					m_ParametersToCreate["DirLight"][v].emplace<std::shared_ptr<DirLight>>(parameter);
				}
			}
			else if (kv.first == "PointLight")
			{
				for (auto v : kv.second)
				{
					std::vector<std::shared_ptr<PointLight>> parameter;
					m_ParametersToCreate["PointLight"][v].emplace<std::vector<std::shared_ptr<PointLight>>>(parameter);
				}
			}
		}
		return m_ParametersToCreate;
	}

	void OpenGLShader::SetParameter(std::string& gameObjectName, GameObjectLibrary& gameObjectLibrary, Camera& camera, DirLight& dirLight, std::vector<PointLight>& pointLights)
	{
		auto gameObject = gameObjectLibrary.Get(gameObjectName);
		for (auto& kv : m_ParametersToCreate)
		{
			if (kv.first == "mat4")
			{
				for (auto& np : kv.second)
				{
					if (np.first == "u_Model")
						np.second = gameObject->GetModel();
					else if (np.first == "u_View")
						np.second = camera.GetViewMatrix();
					else if (np.first == "u_Projection")
						np.second = camera.GetProjectionMatrix();
				}
			}
			else if (kv.first == "vec3")
			{
				for (auto& np : kv.second)
				{
					if (np.first == "u_CameraPos")
					{
						np.second.emplace<glm::vec3>(camera.GetPosition());
						/*for (int i = 0; i < 3; i++)
						{
							DG_CORE_TRACE("{0}:{1}", np.first, glm::value_ptr(std::any_cast<glm::vec3>(np.second))[i]);
						}*/
					}
					else if (np.first == "u_LightDir")
					{
						np.second.emplace<glm::vec3>(dirLight.GetDirection());
						/*for (int i = 0; i < 3; i++)
						{
							DG_CORE_TRACE("{0}:{1}", np.first, glm::value_ptr(std::any_cast<glm::vec3>(np.second))[i]);
						}*/
					}
				}
			}
			else if (kv.first == "DirLight")
			{
				for (auto& np : kv.second)
				{
					np.second.emplace<DirLight>(dirLight);
				}
			}
			else if (kv.first == "PointLight")
			{
				for (auto& np : kv.second)
				{
					np.second.emplace<std::vector<PointLight>>(pointLights);
				}
			}
		}
	}

	void OpenGLShader::UploadAllUniformParameter()
	{
		for (auto kv : m_ParametersToCreate)
		{
			if (kv.first == "float")
			{
				for (auto np : kv.second)
				{
					UploadUniformFloat(np.first, std::any_cast<float>(np.second));
				}
			}
			else if (kv.first == "int")
			{
				for (auto np : kv.second)
				{
					UploadUniformInt(np.first, std::any_cast<int>(np.second));
				}
			}
			else if (kv.first == "bool")
			{
				for (auto np : kv.second)
				{
					UploadUniformBool(np.first, std::any_cast<bool>(np.second));
				}
			}
			else if (kv.first == "vec2")
			{
				for (auto np : kv.second)
				{
					UploadUniformFloat2(np.first, std::any_cast<glm::vec2>(np.second));
				}
			}
			else if (kv.first == "vec3")
			{
				for (auto np : kv.second)
				{
					UploadUniformFloat3(np.first, std::any_cast<glm::vec3>(np.second));
				}
			}
			else if (kv.first == "vec4")
			{
				for (auto np : kv.second)
				{
					UploadUniformFloat4(np.first, std::any_cast<glm::vec4>(np.second));
				}
			}
			else if (kv.first == "mat3")
			{
				for (auto np : kv.second)
				{
					UploadUniformMat3(np.first, std::any_cast<glm::mat3>(np.second));
				}
			}
			else if (kv.first == "mat4")
			{
				for (auto np : kv.second)
				{
					UploadUniformMat4(np.first, std::any_cast<glm::mat4>(np.second));
				}
			}
			else if (kv.first == "sampler2D")
			{
				for (auto np : kv.second)
				{
					UploadUniformInt(np.first, std::any_cast<int>(np.second));
					//DG_CORE_TRACE("{0}:{1}", np.first, std::any_cast<int>(np.second));
				}
			}
			else if (kv.first == "samplerCube")
			{
				for (auto np : kv.second)
				{
					UploadUniformInt(np.first, std::any_cast<int>(np.second));
				}
			}
			else if (kv.first == "DirLight")
			{
				for (auto np : kv.second)
				{
					DG_CORE_INFO("DirLight Yes");
					UploadUniformFloat3(np.first+".direction", std::any_cast<DirLight>(np.second).GetDirection());
					UploadUniformFloat3(np.first + ".diffuse", std::any_cast<DirLight>(np.second).GetDiffuse());
					UploadUniformFloat3(np.first + ".specular", std::any_cast<DirLight>(np.second).GetSpecular());
				}
			}
			else if (kv.first == "PointLight")
			{
				int index = 0;
				for (auto np : kv.second)
				{
					for (auto light : std::any_cast<std::vector<PointLight>>(np.second))
					{
						UploadUniformFloat3(np.first + "[ " + std::to_string(index) + "].position", light.GetPosition());
						UploadUniformFloat3(np.first + "[" + std::to_string(index) + "].diffuse", light.GetDiffuse());
						UploadUniformFloat3(np.first + "[" + std::to_string(index) + "].specular", light.GetSpecular());
						UploadUniformFloat3(np.first + "[" + std::to_string(index) + "].attenuation", light.GetAttenuation());
						index++;
					}
				}
			}
		}
	}



	void OpenGLShader::Compile(const std::unordered_map<unsigned int, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::vector<unsigned int> glShaderIDs(shaderSources.size());

		for (auto& kv : shaderSources)
		{
			unsigned int type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				DG_CORE_ERROR("{0}", infoLog.data());
				DG_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		m_RendererID = program;

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			DG_CORE_ERROR("{0}", infoLog.data());
			DG_CORE_ASSERT(false, "Shader program linking failure!");
		}
		for (auto id : glShaderIDs)
			glDetachShader(program, id);
	}

	void OpenGLShader::ShaderParameterMapInit()
	{
		std::vector<std::string> init = {};
		m_ShaderParametersName["float"] = init;
		m_ShaderParametersName["double"] = init;
		m_ShaderParametersName["int"] = init;
		m_ShaderParametersName["bool"] = init;
		m_ShaderParametersName["vec2"] = init;
		m_ShaderParametersName["vec3"] = init;
		m_ShaderParametersName["vec4"] = init;
		m_ShaderParametersName["mat2"] = init;
		m_ShaderParametersName["mat3"] = init;
		m_ShaderParametersName["mat4"] = init;
		m_ShaderParametersName["sampler2D"] = init;
		m_ShaderParametersName["samplerCube"] = init;
		m_ShaderParametersName["DirLight"] = init;
		m_ShaderParametersName["PointLight"] = init;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::UploadUniformBool(const std::string& name, bool value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}