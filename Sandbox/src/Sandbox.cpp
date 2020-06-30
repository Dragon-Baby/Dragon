#include <Dragon.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Dragon::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),m_CameraController(1280.0/720.f)
	{
		
		m_VertexArray.reset(Dragon::VertexArray::Create());
		float vertices[3 * 6] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f, 0.5f, 1.0f
		};

		std::shared_ptr<Dragon::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Dragon::VertexBuffer::Create(vertices, sizeof(vertices)));
		Dragon::BufferLayout layout = {
			{Dragon::ShaderDataType::Float3, "a_Position"},
			{Dragon::ShaderDataType::Float2, "a_TexCoords"}
		};

		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<Dragon::IndexBuffer> indexBuffer;
		indexBuffer.reset(Dragon::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		//auto textureShader = m_ShaderLibrary.Load("assets/shaders/texture.glsl");

		m_Texture = Dragon::Texture2D::Create("assets/textures/wall.jpg");

		//std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->Bind();
		//std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Dragon::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Dragon::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1.0f});
		Dragon::RenderCommand::Clear();

		Dragon::Renderer::BeginScene();

		//glm::mat4 transform = glm::mat4(1.0f);
		//auto textureShader = m_ShaderLibrary.Get("texture");
		//std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->Bind();
		//m_Texture->Bind(0);
		//std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->UploadUniformMat4("u_Transform", transform);
		//std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->UploadUniformMat4("u_View", m_CameraController.GetCamera().GetViewMatrix());
		//std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->UploadUniformMat4("u_Projection", m_CameraController.GetCamera().GetProjectionMatrix());
		Dragon::Renderer::Submit(m_VertexArray);

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
	std::shared_ptr<Dragon::VertexArray>m_VertexArray;

	std::shared_ptr<Dragon::Texture2D> m_Texture;

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