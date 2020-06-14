#include "dgpch.h"
#include "Application.h"

#include "Dragon/Events/ApplicationEvent.h"
#include "Dragon/Log.h"

#include <GLFW/glfw3.h>

namespace Dragon
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}