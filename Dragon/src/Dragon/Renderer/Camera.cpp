#include "dgpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dragon
{
	Camera::Camera(glm::vec3 position, glm::vec3 up, float nearPlane, float farPlane)
		:m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		m_Position = position;
		m_WorldUp = up;
		m_Up = m_WorldUp;

	}
	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float nearPlane, float farPlane)
		: m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		m_Position = glm::vec3(posX, posY, posZ);
		m_WorldUp = glm::vec3(upX, upY, upZ);
		m_Up = m_WorldUp;
	}

}
