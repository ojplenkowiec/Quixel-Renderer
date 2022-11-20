#include "camera.h"

Camera::Camera(float screenAspectRatio, float fov, float nearDistance, float farDistance)
{
	c_PositionVector = glm::vec3(10000.0f, 10000.0f, 10000.0f);

	c_DirectionVector = glm::vec3(0.0f, 0.0f, -1.0f);
	c_UpVector = glm::vec3(0.0f, 1.0f, 0.0f); // default values! make modifiable?
	c_RightVector = glm::vec3(1.0f, 0.0f, 0.0f);

	c_Pitch = 0.0f;
	c_Yaw = -90.0f;

	c_Fov = 65.0f;
	c_AspectRatio = screenAspectRatio;
	c_Near = nearDistance;
	c_Far = farDistance;

	UpdateDirectionVectors(); // might not be necessary but ah well...
}



void Camera::MoveForwards(float distance)
{
	c_PositionVector += c_DirectionVector * distance;
}

void Camera::MoveRight(float distance)
{
	c_PositionVector += c_RightVector * distance;
}

void Camera::MoveUp(float distance)
{
	c_PositionVector += c_UpVector * distance;
}

void Camera::Rotate(float addedPitch, float addedYaw)
{
	c_Pitch += addedPitch;
	c_Yaw += addedYaw;

	// LIMIT UP/DOWN
	if (c_Pitch > 89.0f) {
		c_Pitch = 89.0f;
	}
	else if (c_Pitch < -89.0f) {
		c_Pitch = -89.0f;
	}

	UpdateDirectionVectors();
}



void Camera::TranslateForwards(float distance)
{
	c_PositionVector += glm::vec3(0.0f, 0.0f, distance);
}

void Camera::TranslateRight(float distance)
{
	c_PositionVector += glm::vec3(distance, 0.0f, 0.0f);
}

void Camera::TranslateUp(float distance)
{
	c_PositionVector += glm::vec3(0.0f, distance, 0.0f);
}

void Camera::Translate(glm::vec3 translationVector)
{
	c_PositionVector += translationVector;
}



glm::mat4 Camera::GetProjectionMatrix()
{
	return (glm::perspective(glm::radians(c_Fov), c_AspectRatio, c_Near, c_Far));
}

glm::mat4 Camera::GetViewMatrix()
{
	return (glm::lookAt(c_PositionVector, c_PositionVector + c_DirectionVector, c_UpVector));
}

void Camera::UpdateDirectionVectors()
{
	c_DirectionVector.x = cos(glm::radians(c_Yaw)) * cos(glm::radians(c_Pitch));
	c_DirectionVector.y = sin(glm::radians(c_Pitch));
	c_DirectionVector.z = sin(glm::radians(c_Yaw)) * cos(glm::radians(c_Pitch));

	c_DirectionVector = glm::normalize(c_DirectionVector);

	c_UpVector.x = cos(glm::radians(c_Yaw)) * cos(glm::radians(c_Pitch + 90.0f));
	c_UpVector.y = sin(glm::radians(c_Pitch + 90.0f));
	c_UpVector.z = sin(glm::radians(c_Yaw)) * cos(glm::radians(c_Pitch + 90.0f));

	c_UpVector = glm::normalize(c_UpVector);

	c_RightVector = glm::cross(c_DirectionVector, c_UpVector);

	c_RightVector = glm::normalize(c_RightVector);
}