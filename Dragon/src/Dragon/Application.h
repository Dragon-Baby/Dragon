#pragma once

#include "Core.h"

#include "Window.h"
#include "Dragon/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Dragon/ImGui/ImGuiLayer.h"

#include "Dragon/Renderer/Shader.h"
#include "Dragon/Renderer/Buffer.h"
#include "Dragon/Renderer/VertexArray.h"


namespace Dragon
{
	class DRAGON_API Application
	{
	public :
		Application();

		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray>m_VertexArray;
	private:
		static Application* s_Instance;
	};

	// to be defined in the client
	Application* CreateApplication();
}

