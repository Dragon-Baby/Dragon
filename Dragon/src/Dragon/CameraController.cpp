#include "dgpch.h"
#include "CameraController.h"

#include "Dragon/Input.h"
#include "Dragon/KeyCodes.h"

#include <GLFW/glfw3.h>

namespace Dragon
{
	CameraController::CameraController(float aspectRatio)
		:m_AspectRatio(aspectRatio)
	{
		m_Camera.SetPosition(m_CameraPosition);
	}
	void CameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(DG_KEY_LEFT))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(DG_KEY_RIGHT))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(DG_KEY_UP))
			m_CameraPosition.z -= m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(DG_KEY_DOWN))
			m_CameraPosition.z += m_CameraTranslationSpeed * ts;

		m_Camera.SetPosition(m_CameraPosition);

		//m_CameraTranslationSpeed = m_ZoomLevel;
	}
	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(DG_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(DG_BIND_EVENT_FN(CameraController::OnWindowResized));
		dispatcher.Dispatch<MouseMovedEvent>(DG_BIND_EVENT_FN(CameraController::OnMouseMoved));
	}
	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= (e.GetYOffset() * 0.25f);
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		//m_Camera.SetAspectRatio(m_Camera.GetAspectRatio() * m_ZoomLevel);
		m_Camera.SetZoom(m_ZoomLevel);

		return false;
	}
	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_Camera.SetAspectRatio(float(e.GetWidth()) / float(e.GetHeight()));
		return false;
	}
	bool CameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		float xPos, yPos;
		xPos = e.GetX();
		yPos = e.GetY();
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		float xOffset = 0.0f;
		float yOffset = 0.0f;
		xOffset = (xPos - lastX) * m_MouseSensitivity;
		yOffset = (lastY - yPos) * m_MouseSensitivity;
		lastX = xPos;
		lastY = yPos;
		m_Yaw += xOffset;
		m_Pitch += yOffset;
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		UpdateCameraVectors();
		return false;
	}
	void CameraController::UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_Camera.SetFront(glm::normalize(front));
		m_Camera.SetRight(glm::normalize(glm::cross(m_Camera.GetFront(), m_Camera.GetWorldUp())));
		m_Camera.SetUp(glm::cross(m_Camera.GetRight(), m_Camera.GetFront()));
	}
}