#include <Dragon.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Dragon::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		camera.SetPosition(glm::vec3(0, 0, 1.0));

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
			{Dragon::ShaderDataType::Float2, "a_TexCoord"}
		};

		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<Dragon::IndexBuffer> indexBuffer;
		indexBuffer.reset(Dragon::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/texture.glsl");

		m_Texture = Dragon::Texture2D::Create("assets/textures/wall.jpg");

		std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Dragon::Timestep ts) override
	{
		Dragon::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1.0f});
		Dragon::RenderCommand::Clear();

		Dragon::Renderer::BeginScene();

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		auto textureShader = m_ShaderLibrary.Get("texture");
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->Bind();
		m_Texture->Bind(0);
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->UploadUniformMat4("u_Transform", scale);
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->UploadUniformMat4("u_View", camera.GetViewMatrix());
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(textureShader)->UploadUniformMat4("u_Projection", camera.GetProjectionMatrix());
		Dragon::Renderer::Submit(m_VertexArray);

		Dragon::Renderer::EndScene();
	}
	virtual void OnImGuiRender() override
	{

	}


	void OnEvent(Dragon::Event& event) override
	{
		
	}

private:
	Dragon::ShaderLibrary m_ShaderLibrary;
	std::shared_ptr<Dragon::VertexArray>m_VertexArray;

	std::shared_ptr<Dragon::Texture2D> m_Texture;

	Dragon::Camera camera;

	
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