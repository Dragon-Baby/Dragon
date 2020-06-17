#include <Dragon.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Dragon::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		if (Dragon::Input::IsKeyPressed(DG_KEY_TAB))
			DG_TRACE("Tab key is pressed (poll)!");
	}


	void OnEvent(Dragon::Event& event) override
	{
		if (event.GetEventType() == Dragon::EventType::KeyPressed)
		{
			Dragon::KeyPressedEvent& e = (Dragon::KeyPressedEvent&)event;
			if (e.GetKeyCode() == DG_KEY_TAB)
				DG_TRACE("Tab key is pressed (event)!");
			DG_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
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