#include "camera2d.h"

Camera2D::Camera2D(float width, float height)
{
	m_Width = width;
	m_Height = height;

	m_PositionVector = glm::vec2(width / 2.0f, height / 2.0f);

	m_UpVector = glm::vec2(0.0f, 1.0f);
	m_RightVector = glm::vec2(1.0f, 0.0f);

	c_Roll = 0.0f;

	m_ProjectionMatrixNeedsUpdate = true;
	m_ViewMatrixNeedsUpdate = true;

	m_ProjectionMatrix = glm::mat4(1.0f);
	m_ViewMatrix = glm::mat4(1.0f);
}

void Camera2D::UpdateDirectionVectors()
{
	float theta = glm::radians(c_Roll);
	glm::mat2 rotationMatrix = glm::mat2(cos(theta), -sin(theta), sin(theta), cos(theta)); // column order
	m_UpVector = rotationMatrix * glm::vec2(0.0f, 1.0f);
	m_RightVector = rotationMatrix * glm::vec2(1.0f, 0.0f);
}

void Camera2D::UpdateViewMatrix()
{
	m_ViewMatrix = glm::lookAt(glm::vec3(m_PositionVector.x, m_PositionVector.y, -1.0f), glm::vec3(m_PositionVector.x, m_PositionVector.y, 0.0f), glm::vec3(m_UpVector.x, m_UpVector.y, -1.0f));
}

void Camera2D::UpdateProjectionMatrix()
{
	m_ProjectionMatrix = glm::ortho(m_Width / 2.0f, -m_Width / 2.0f, -m_Height / 2.0f, m_Height / 2.0f, 0.1f, 100.0f);
}

void Camera2D::Move(glm::vec2 moveVector)
{
	m_PositionVector += m_RightVector * moveVector.x;
	m_PositionVector += m_UpVector * moveVector.y;
}

void Camera2D::Rotate(float angle)
{
	c_Roll += angle;
	UpdateDirectionVectors();
}

void Camera2D::Translate(glm::vec2 translationVector)
{
	m_PositionVector += translationVector;
}

glm::mat4 Camera2D::GetProjectionMatrix()
{
	if (m_ProjectionMatrixNeedsUpdate) {
		UpdateProjectionMatrix();
	}
	return m_ProjectionMatrix;
}

glm::mat4 Camera2D::GetViewMatrix()
{
	if (m_ViewMatrixNeedsUpdate) {
		UpdateViewMatrix();
	}
	return m_ViewMatrix;
}
