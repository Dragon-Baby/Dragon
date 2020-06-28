#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dragon
{
	enum CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// 摄像机默认参数
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;

	class Camera
	{
	public:
		Camera(glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~Camera() = default;

		inline glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }
		inline glm::mat4 GetProjectionMatrix() {return glm::perspective((float)glm::radians(Zoom), 1280.0f/720.0f, 0.1f, 100.0f);}
		void ProcessKeyboard(CameraMovement direction, float deltaTime);
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void ProcessMouseScroll(float yoffset);
		glm::vec3 GetPosition() { return Position; }
		void SetPosition(glm::vec3 position) { Position = position; }

	private:
		void updateCameraVectors();
	private:
		//摄像机参数
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		//欧拉角
		float Yaw;
		float Pitch;

		//摄像机选项
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;
	};

}
