#include <Dragon.h>

class ExampleLayer : public Dragon::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		DG_INFO("ExampleLayer::Update");
	}

	void OnEvent(Dragon::Event& event) override
	{
		DG_TRACE("{0}", event);
	}
};

class Sandbox : public Dragon::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Dragon::ImGuiLayer());
	}
	
	~Sandbox()
	{

	}
};

Dragon::Application* Dragon::CreateApplication()
{
	return new Sandbox();
}