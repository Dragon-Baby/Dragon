#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dragon
{
	class Camera
	{
	public:
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float farPlane = 0.1f, float nearPlane = 100.0f);
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float nearPlane, float farPlane);
		~Camera() = default;

		Camera(Camera& camera) 
		{
			m_Position = camera.m_Position;
			m_Front = camera.m_Front;
			m_Up = camera.m_Up;
			m_Right = camera.m_Right;
			m_WorldUp = camera.m_WorldUp;
			m_Zoom = camera.m_Zoom;
			m_NearPlane = camera.m_NearPlane;
			m_FarPlane = camera.m_FarPlane;
			m_AspectRatio = camera.m_AspectRatio;

		}

		inline glm::mat4 GetViewMatrix() { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
		inline glm::mat4 GetProjectionMatrix() {return glm::perspective((float)glm::radians(m_Zoom), m_AspectRatio, m_NearPlane, m_FarPlane);}
		glm::vec3 GetPosition() { return m_Position; }
		void SetPosition(glm::vec3 position) { m_Position = position; }

		glm::vec3 GetFront() { return m_Front; }
		void SetFront(glm::vec3 front) { m_Front = front; }

		glm::vec3 GetUp() { return m_Up; }
		void SetUp(glm::vec3 up) { m_Up = up; }

		glm::vec3 GetRight() { return m_Right; }
		void SetRight(glm::vec3 right) { m_Right = right; }

		glm::vec3 GetWorldUp() { return m_WorldUp; }
		void SetWorldUp(glm::vec3 worldUp) { m_WorldUp = worldUp; }

		float GetZoom() { return m_Zoom; }
		void SetZoom(float zoom) { m_Zoom = zoom; }

		float GetAspectRatio() { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

		float GetNearPlane() { return m_NearPlane; }
		void SetNearPlane(float nearPlane) { m_NearPlane = nearPlane; }

		float GetFarPlane() { return m_FarPlane; }
		void SetFarPlane(float farPlane) { m_FarPlane = farPlane; }
	private:
		//ÉãÏñ»ú²ÎÊý
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
		float m_NearPlane;
		float m_FarPlane;
		float m_AspectRatio = 1280.0f/720.f;

		float m_Zoom = 45.0f;
	};

}
