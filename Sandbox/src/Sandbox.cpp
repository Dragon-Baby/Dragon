#include <Dragon.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Dragon::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Dragon::VertexArray::Create());

		float vertices[3 * 6] = {
			-0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Dragon::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Dragon::VertexBuffer::Create(vertices, sizeof(vertices)));

		Dragon::BufferLayout layout = {
			{Dragon::ShaderDataType::Float3, "a_Position"},
			{Dragon::ShaderDataType::Float3, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<Dragon::IndexBuffer> indexBuffer;
		indexBuffer.reset(Dragon::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;

			uniform mat4 u_ViewPorjection;
			
			out vec3 v_Position;
			out vec3 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewPorjection * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core
			out vec4 color;

			in vec3 v_Position;
			in vec3 v_Color;
			void main()
			{
				color = vec4(v_Color, 1.0);
			}

		)";

		m_Shader.reset(new Dragon::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate(Dragon::Timestep ts) override
	{


		Dragon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Dragon::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Dragon::Renderer::BeginScene(m_Camera);

		m_Shader->Bind();
		m_Shader->UploadUniformMat4("u_ViewPorjection", m_Camera.GetViewProjectionMatrix());
		Dragon::Renderer::Submit(m_VertexArray);

		Dragon::Renderer::EndScene();
	}


	void OnEvent(Dragon::Event& event) override
	{
		
	}

private:
	std::shared_ptr<Dragon::Shader> m_Shader;
	std::shared_ptr<Dragon::VertexArray>m_VertexArray;

	Dragon::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;

	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;
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