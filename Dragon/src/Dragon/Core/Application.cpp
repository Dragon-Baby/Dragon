#include "dgpch.h"
#include "Application.h"

#include "Dragon/Core/Log.h"

#include "Dragon/Renderer/Renderer.h"
#include "Input.h"
#include "KeyCodes.h"

#include <GLFW/glfw3.h>


namespace Dragon
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		DG_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyBoard));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			if(!m_Cursor)
				glfwSetInputMode((GLFWwindow*)m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			else
				glfwSetInputMode((GLFWwindow*)m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnKeyBoard(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == DG_KEY_Q)
		{
			if (!m_Cursor)
				m_Cursor = true;
			else
				m_Cursor = false;
		}
		return false;
	}
}