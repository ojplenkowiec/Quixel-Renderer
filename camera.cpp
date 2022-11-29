#include "camera.h"

Camera::Camera(float screenAspectRatio, float fov, float nearDistance, float farDistance)
{
	m_PositionVector = glm::vec3(0.0f, 150.0f, 0.0f);

	m_ForwardsVector = glm::vec3(0.0f, 0.0f, -1.0f);
	m_UpVector = glm::vec3(0.0f, 1.0f, 0.0f); // default values! make modifiable?
	m_RightVector = glm::vec3(1.0f, 0.0f, 0.0f);

	c_Pitch = 0.0f;
	c_Yaw = -90.0f;

	m_FieldOfView = 65.0f;
	m_AspectRatio = screenAspectRatio;
	m_NearClipDistance = nearDistance;
	m_FarClipDistance = farDistance;

	m_MaxFov = 132.0f;
	m_MinFov = 5.0f;

	UpdateDirectionVectors(); // might not be necessary but ah well...

	m_ProjectionMatrixNeedsUpdate = true;
	m_ViewMatrixNeedsUpdate = true;
}

void Camera::UpdateFov(float dFov)
{
	m_FieldOfView += dFov;
	if (m_FieldOfView > m_MaxFov) {
		m_FieldOfView = m_MaxFov;
	}
	else if (m_FieldOfView < m_MinFov) {
		m_FieldOfView = m_MinFov;
	}
	m_ProjectionMatrixNeedsUpdate = true;
}

void Camera::SetFov(float fov)
{
	m_FieldOfView = fov;
	if (m_FieldOfView > m_MaxFov) {
		m_FieldOfView = m_MaxFov;
	}
	else if (m_FieldOfView < m_MinFov) {
		m_FieldOfView = m_MinFov;
	}
	m_ProjectionMatrixNeedsUpdate = true;
}



void Camera::MoveForwards(float distance)
{
	m_PositionVector += m_ForwardsVector * distance;
	m_ViewMatrixNeedsUpdate = true;
}

void Camera::MoveRight(float distance)
{
	m_PositionVector += m_RightVector * distance;
	m_ViewMatrixNeedsUpdate = true;
}

void Camera::MoveUp(float distance)
{
	m_PositionVector += m_UpVector * distance;
	m_ViewMatrixNeedsUpdate = true;
}

void Camera::Rotate(float addedPitch, float addedYaw)
{
	c_Pitch += addedPitch;
	c_Yaw += addedYaw;

	if (c_Pitch > 89.0f) {
		c_Pitch = 89.0f;
	}
	else if (c_Pitch < -89.0f) {
		c_Pitch = -89.0f;
	}

	UpdateDirectionVectors(); // maybe also make into a status for update?
	m_ViewMatrixNeedsUpdate = true;
}



void Camera::TranslateForwards(float distance)
{
	m_PositionVector += glm::vec3(0.0f, 0.0f, distance);
	m_ViewMatrixNeedsUpdate = true;
}

void Camera::TranslateRight(float distance)
{
	m_PositionVector += glm::vec3(distance, 0.0f, 0.0f);
	m_ViewMatrixNeedsUpdate = true;
}

void Camera::TranslateUp(float distance)
{
	m_PositionVector += glm::vec3(0.0f, distance, 0.0f);
	m_ViewMatrixNeedsUpdate = true;
}

void Camera::Translate(glm::vec3 translationVector)
{
	m_PositionVector += translationVector;
	m_ViewMatrixNeedsUpdate = true;
}



void Camera::FocusOn(glm::vec3 position) // update this
{
	m_PositionVector = position;
	m_ViewMatrixNeedsUpdate = true;
}



glm::vec3 Camera::GetRay(float mouse_x, float mouse_y, float windowWidth, float windowHeight)
{
	float x = (2.0f * mouse_x) / windowWidth - 1.0f;
	float y = ((2.0f * mouse_y) / windowHeight - 1.0f) * -1.0f;
	float z = 1.0f;
	glm::vec3 rayNds = glm::vec3(x, y, z);
	glm::vec4 rayClip = glm::vec4(rayNds.x, rayNds.y, -1.0f, -1.0f);
	glm::vec4 rayEye = glm::inverse(GetProjectionMatrix()) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
	glm::vec4 rayWorldVec4 = (glm::inverse(GetViewMatrix()) * rayEye);
	return glm::normalize(glm::vec3(rayWorldVec4.x, rayWorldVec4.y, rayWorldVec4.z)) * 500000.0f;
}



void Camera::UpdateDirectionVectors()
{
	m_ForwardsVector.x = cos(glm::radians(c_Yaw)) * cos(glm::radians(c_Pitch));
	m_ForwardsVector.y = sin(glm::radians(c_Pitch));
	m_ForwardsVector.z = sin(glm::radians(c_Yaw)) * cos(glm::radians(c_Pitch));

	m_ForwardsVector = glm::normalize(m_ForwardsVector);

	m_UpVector.x = cos(glm::radians(c_Yaw)) * cos(glm::radians(c_Pitch + 90.0f));
	m_UpVector.y = sin(glm::radians(c_Pitch + 90.0f));
	m_UpVector.z = sin(glm::radians(c_Yaw)) * cos(glm::radians(c_Pitch + 90.0f));

	m_UpVector = glm::normalize(m_UpVector);

	m_RightVector = glm::cross(m_ForwardsVector, m_UpVector);

	m_RightVector = glm::normalize(m_RightVector);
}

void Camera::UpdateViewMatrix()
{
	m_ViewMatrix = glm::lookAt(m_PositionVector, m_PositionVector + m_ForwardsVector, m_UpVector);
}

void Camera::UpdateProjectionMatrix()
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FieldOfView), m_AspectRatio, m_NearClipDistance, m_FarClipDistance);
}



glm::mat4 Camera::GetProjectionMatrix()
{
	if (m_ProjectionMatrixNeedsUpdate) {
		UpdateProjectionMatrix();
		m_ProjectionMatrixNeedsUpdate = false;
	}
	return m_ProjectionMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
	if (m_ViewMatrixNeedsUpdate) {
		UpdateViewMatrix();
		m_ViewMatrixNeedsUpdate = false;
	}
	return m_ViewMatrix;
}