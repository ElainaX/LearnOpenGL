#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), 
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
		float pitch = PITCH) 
			: Front(glm::vec3(0.0f, 0.0f, -1.0f)), Speed(SPEED), 
				Sensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		Global_Up = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();

	}

	void processMouseMovement(float xOffset, float yOffset)
	{
		Yaw += xOffset;
		Pitch += yOffset;

		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;

		float x = std::cos(glm::radians(Pitch)) * std::sin(glm::radians(Yaw));
		float y = std::sin(glm::radians(Pitch));
		float z = -std::cos(glm::radians(Pitch)) * std::cos(glm::radians(Yaw));
		Front = glm::normalize(glm::vec3{ x, y, z });
	}
public:
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Global_Up;
	glm::vec3 Right;
	glm::vec3 Front;
	float Speed;
	float Pitch = 0;
	float Yaw = 0;
	float Sensitivity;
	float Zoom;
	float fov = 45.0f;

private:
	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = -cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, Global_Up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
