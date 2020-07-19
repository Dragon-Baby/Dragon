#include "dgpch.h"
#include "CameraController.h"

#include "Dragon/Core/Input.h"
#include "Dragon//Core/KeyCodes.h"

#include "Dragon/Core/Application.h"

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
		m_Timestep = ts;
		UpdateCameraVectors();
		//m_CameraTranslationSpeed = m_ZoomLevel;
	}
	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(DG_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(DG_BIND_EVENT_FN(CameraController::OnWindowResized));
		dispatcher.Dispatch<MouseMovedEvent>(DG_BIND_EVENT_FN(CameraController::OnMouseMoved));
		dispatcher.Dispatch<KeyPressedEvent>(DG_BIND_EVENT_FN(CameraController::OnKeyBoard));
		dispatcher.Dispatch<MouseButtonPressedEvent>(DG_BIND_EVENT_FN(CameraController::OnMouseClicked));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(DG_BIND_EVENT_FN(CameraController::OnMouseReleased));
		dispatcher.Dispatch<KeyReleasedEvent>(DG_BIND_EVENT_FN(CameraController::OnKeyReleased));
	}
	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (m_ZoomLevel >= 1.0f && m_ZoomLevel <= 90.0f)
			m_ZoomLevel -= e.GetYOffset();
		if (m_ZoomLevel <= 1.0f)
			m_ZoomLevel = 1.0f;
		if (m_ZoomLevel >= 90.0f)
			m_ZoomLevel = 90.0f;
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
		if (!m_Quit)
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
			if (m_ALT)
			{
				if (m_MouseLeft)
				{
					m_Yaw += xOffset*5;
					m_Pitch += yOffset*5;
					if (m_Pitch > 89.0f)
						m_Pitch = 89.0f;
					if (m_Pitch < -89.0f)
						m_Pitch = -89.0f;
				}
				if (m_MouseMiddle)
				{
					m_Camera.SetPosition(m_Camera.GetPosition() - m_Camera.GetRight() * (xOffset*20 * m_Timestep));
					m_Camera.SetPosition(m_Camera.GetPosition() - m_Camera.GetUp() * (yOffset *20* m_Timestep));
				}
			}
		}
		else if (m_Quit)
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
		}

		return false;
	}

	bool CameraController::OnKeyBoard(KeyPressedEvent& e)
	{
		if (m_Quit)
		{
			if (e.GetKeyCode() == DG_KEY_A)
					m_Camera.SetPosition(m_Camera.GetPosition() - m_Camera.GetRight() * (m_CameraTranslationSpeed * m_Timestep));
			if (e.GetKeyCode() == DG_KEY_D)
				m_Camera.SetPosition(m_Camera.GetPosition() + m_Camera.GetRight() * (m_CameraTranslationSpeed * m_Timestep));
			if (e.GetKeyCode() == DG_KEY_W)
				m_Camera.SetPosition(m_Camera.GetPosition() + m_Camera.GetFront() * (m_CameraTranslationSpeed * m_Timestep));
			if (e.GetKeyCode() == DG_KEY_S)
				m_Camera.SetPosition(m_Camera.GetPosition() - m_Camera.GetFront() * (m_CameraTranslationSpeed * m_Timestep));
		}
		if (e.GetKeyCode() == DG_KEY_Q)
		{
			if (m_Quit)
			{
				m_LastCamera = m_Camera;
				firstMouse = true;
				m_Quit = !m_Quit;
			}
			else
			{
				m_Camera = m_LastCamera;
				m_Quit = !m_Quit;
			}
		}
		if (!m_Quit)
		{
			if (e.GetKeyCode() == DG_KEY_LEFT_CONTROL)
				m_CTRL = true;
			if (e.GetKeyCode() == DG_KEY_LEFT_ALT)
				m_ALT = true;
		}
		return false;
	}

	bool CameraController::OnKeyReleased(KeyReleasedEvent& e)
	{
		if (!m_Quit)
		{
			if (e.GetKeyCode() == DG_KEY_LEFT_CONTROL)
				m_CTRL = false;
			if (e.GetKeyCode() == DG_KEY_LEFT_ALT)
				m_ALT = false;
		}
		return false;
	}

	bool CameraController::OnMouseClicked(MouseButtonPressedEvent& e)
	{
		if (!m_Quit)
		{
			if (e.GetMouseButton() == DG_MOUSE_BUTTON_LEFT)
				m_MouseLeft = true;
			if (e.GetMouseButton() == DG_MOUSE_BUTTON_RIGHT)
				m_MouseRight = true;
			if (e.GetMouseButton() == DG_MOUSE_BUTTON_MIDDLE)
				m_MouseMiddle = true;
		}
		return false;
	}

	bool CameraController::OnMouseReleased(MouseButtonReleasedEvent& e)
	{
		if (!m_Quit)
		{
			if (e.GetMouseButton() == DG_MOUSE_BUTTON_LEFT)
				m_MouseLeft = false;
			if (e.GetMouseButton() == DG_MOUSE_BUTTON_RIGHT)
				m_MouseRight = false;
			if (e.GetMouseButton() == DG_MOUSE_BUTTON_MIDDLE)
				m_MouseMiddle = false;
		}
		return false;
	}


	void CameraController::UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_Camera.SetFront(glm::normalize(front));
		m_Camera.SetWorldUp(glm::vec3(0.0f, 1.0f, 0.0f));
		m_Camera.SetRight(glm::normalize(glm::cross(m_Camera.GetFront(), m_Camera.GetWorldUp())));
		m_Camera.SetUp(glm::cross(m_Camera.GetRight(), m_Camera.GetFront()));
	}
}
