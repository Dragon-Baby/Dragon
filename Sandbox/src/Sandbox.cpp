#include <Dragon.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include "Build_in/GameObjects.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

static const char* m_ShaderList[]{ 
	"Lambert",
	"LambertTexture",
	"Phong", 
	"PhongTexture",
	"Blinn", 
	"BlinnTexture",  
	"PBR",
	"PBRTexture",
	"AlphaTest",
	"AlphaBlending",
	"ShadowMapDir",
	"ShadowMapPoint",
	"SkyBox"
};

static int item = 0;

class ExampleLayer : public Dragon::Layer
{

public:
	ExampleLayer()
		: Layer("Example"),m_CameraController(1280.0/720.f),sphere(), skybox()
	{

		//point light init
		for (int i = 0; i < m_PointLightNum; i++)
		{
			Dragon::PointLight pointLight;
			m_PointLights.push_back(pointLight);
		}


		//cube init
		m_ShaderLibrary.Load("cubeShader", "assets/shaders/Lambert.glsl");
		auto cubeShader = m_ShaderLibrary.Get("cubeShader");
		cube.Init();

		//skybox init
		m_ShaderLibrary.Load("skybox", "assets/shaders/SkyBox.glsl");
		auto skyBoxShader = m_ShaderLibrary.Get("skybox");
		skybox.SetFaces(m_Faces);
		skybox.Init();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->Bind();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->UploadUniformInt("u_SkyBox", 0);
		glEnable(GL_DEPTH_TEST);
	}

	void OnUpdate(Dragon::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		auto cubeShader = m_ShaderLibrary.Get("cubeShader");
		auto skyBoxShader = m_ShaderLibrary.Get("skybox");

		Dragon::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1.0f});
		Dragon::RenderCommand::Clear();

		Dragon::Renderer::BeginScene();
		//cube
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->Bind();
		//MVP
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformMat4("u_Model", cube.GetModel());
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformMat4("u_Projection", m_CameraController.GetCamera().GetProjectionMatrix());
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformMat4("u_View", m_CameraController.GetCamera().GetViewMatrix());
		//dir liight
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_DirLight.direction", m_DirLight.GetDirection());
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_DirLight.diffuse", m_DirLight.GetDiffuse());
		//point light
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformInt("u_PointLightNum", m_PointLightNum);
		int pointLightIndex = 0;
		for (auto pointLight : m_PointLights)
		{
			pointLight.SetPosition(glm::vec3((10.0f  * pointLightIndex +10, 10.0f  * pointLightIndex+10, 10.0f  * pointLightIndex+10)));
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_PointLights["+ std::to_string(pointLightIndex) +"].position", pointLight.GetPosition());
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_PointLights["+ std::to_string(pointLightIndex) +"].diffuse", pointLight.GetDiffuse());
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_PointLights[" + std::to_string(pointLightIndex) + "].attenuation", pointLight.GetAttenuation());
			pointLightIndex++;
		}
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_Albedo", glm::vec3(0.5f, 0.3f, 0.2f));
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_Ambient", glm::vec3(0.5f, 0.3f, 0.2f));
		cube.Render();
	
		//skybox
		glDepthFunc(GL_LEQUAL);
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->Bind();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->UploadUniformMat4("u_Projection", m_CameraController.GetCamera().GetProjectionMatrix());
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->UploadUniformMat4("u_View", glm::mat4(glm::mat3(m_CameraController.GetCamera().GetViewMatrix())));
		skybox.Render();
		glDepthFunc(GL_LESS);

		Dragon::Renderer::EndScene();
	}
	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Renderer Setting");
		/*ImGui::ColorEdit3("Color Tint", m_Color);
		ImGui::SliderFloat("Metallic", &m_Metallic, 0.0f, 1.0f);
		ImGui::SliderFloat("Roughness", &m_Roughness, 0.0f, 1.0f);
		ImGui::SliderFloat("AO", &m_AO, 0.0f, 1.0f);*/
		ImGui::ListBox("Shader List", &item, m_ShaderList, 13);
		ImGui::End();
	}


	void OnEvent(Dragon::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}
private:
	void ShaderListInit()
	{
		
	}
private:
	Dragon::ShaderLibrary m_ShaderLibrary;

	//GameObjects-----------------------------------
	Dragon::Sphere sphere;
	Dragon::Cube cube;
	Dragon::SkyBox skybox;
	std::vector<std::string> m_Faces = {
		"./assets/textures/skybox/right.jpg",
		"./assets/textures/skybox/left.jpg",
		"./assets/textures/skybox/top.jpg",
		"./assets/textures/skybox/bottom.jpg",
		"./assets/textures/skybox/front.jpg",
		"./assets/textures/skybox/back.jpg",
	};
	
	//Camera----------------------------------------
	Dragon::CameraController m_CameraController;
	Dragon::Camera m_Camera;

	//Light-----------------------------------------
	Dragon::DirLight m_DirLight;
	int m_PointLightNum = 1;
	std::vector<Dragon::PointLight> m_PointLights;

	bool m_Check = false;
	float m_Metallic = 0.0f;
	float m_Roughness = 0.0f;
	float m_AO = 0.0f;
	float m_Color[3] = {0.0f, 0.0f, 0.0f};
};

class Sandbox : public Dragon::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	
	~Sandbox()
	{

	}
};

Dragon::Application* Dragon::CreateApplication()
{
	return new Sandbox();
}