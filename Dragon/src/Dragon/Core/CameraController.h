#pragma once

#include "Dragon/Renderer/Camera.h"
#include "Dragon/Core/Timestep.h"

#include "Dragon/Events/ApplicationEvent.h"
#include "Dragon/Events/MouseEvent.h"
#include "Dragon/Events/KeyEvents.h"

namespace Dragon
{
	class CameraController
	{
	public:
		CameraController(float aspectRatio);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera()const { return m_Camera; }

		Timestep& GetTimestep(){return m_Timestep;};
		void SetTimestep(Timestep& ts) { m_Timestep = ts; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnKeyBoard(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnMouseClicked(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);
		void UpdateCameraVectors();
	private:
		Camera m_Camera;
		Camera m_LastCamera;
		glm::vec3 m_CameraPosition = glm::vec3(0.0f,0.0f,5.0f);
		float m_AspectRatio;
		float m_CameraTranslationSpeed = 5.0f;
		float m_ZoomLevel = 45.0f;
		float m_MouseSensitivity = 0.01f;
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
		bool firstMouse = true;
		float lastX , lastY ;
		Timestep m_Timestep;

		bool m_Quit = true;
		bool m_CTRL = false;
		bool m_ALT = false;
		bool m_MouseLeft = false;
		bool m_MouseRight = false;
		bool m_MouseMiddle = false;
	};
}
