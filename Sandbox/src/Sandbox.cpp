#include <Dragon.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include "Build_in/GameObjects.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

class ExampleLayer : public Dragon::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),m_CameraController(1280.0/720.f),sphere()
	{
		m_Camera = m_CameraController.GetCamera();
		//sphere.Init();
		m_ShaderLibrary.Load("skybox", "assets/shaders/SkyBox.glsl");
		auto skyBoxShader = m_ShaderLibrary.Get("skybox");
		//skybox.SetShader(skyBoxShader);
		skybox.SetFaces(m_Faces);
		skybox.Init();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->Bind();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->UploadUniformInt("u_SkyBox", 0);
		m_Camera.SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		m_Camera.SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
		glEnable(GL_DEPTH_TEST);
	}

	void OnUpdate(Dragon::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);
		auto skyBoxShader = m_ShaderLibrary.Get("skybox");
		Dragon::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1.0f});
		Dragon::RenderCommand::Clear();

		Dragon::Renderer::BeginScene();

		//sphere.Render();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->Bind();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->UploadUniformMat4("u_Projection", m_Camera.GetProjectionMatrix());
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->UploadUniformMat4("u_View", m_Camera.GetViewMatrix());
		skybox.Render();

		Dragon::Renderer::EndScene();
	}
	virtual void OnImGuiRender() override
	{
		ImGui::Begin("window");
		ImGui::ColorPicker3("Color", m_Color);
		ImGui::SliderFloat("Metallic", &m_Metallic, 0.0f, 1.0f);
		ImGui::SliderFloat("Roughness", &m_Roughness, 0.0f, 1.0f);
		ImGui::SliderFloat("AO", &m_AO, 0.0f, 1.0f);
		ImGui::End();
	}


	void OnEvent(Dragon::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

private:
	Dragon::ShaderLibrary m_ShaderLibrary;

	Dragon::Sphere sphere;

	Dragon::SkyBox skybox;
	std::vector<std::string> m_Faces = {
		"assets/textures/skybox/right.jpg",
		"assets/textures/skybox/left.jpg",
		"assets/textures/skybox/top.jpg",
		"assets/textures/skybox/bottom.jpg",
		"assets/textures/skybox/front.jpg",
		"assets/textures/skybox/back.jpg",
	};
	
	Dragon::CameraController m_CameraController;
	Dragon::Camera m_Camera;

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