#ifndef CAMERA_H
#define CAMERA_H

#include "glfw3.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera { // camera should have a target window?
private:
	glm::vec2 m_PositionVector;

	glm::vec2 m_UpVector;
	glm::vec2 m_RightVector;

	float m_Width;
	float m_Height;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	bool m_ViewMatrixNeedsUpdate;
	bool m_ProjectionMatrixNeedsUpdate;

	// float c_Pitch; // up/down
	// float c_Yaw; // left/right
	float c_Roll;

	void UpdateDirectionVectors();

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
public:
	Camera(float width, float height);

	void Move(glm::vec2 moveVector);
	void Rotate(float angle);

	void Translate(glm::vec2 translationVector);

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	inline glm::vec2 GetPosition() { return m_PositionVector; }
	inline glm::vec2 GetUpVector() { return m_UpVector; }
	inline glm::vec2 GetRightVector() { return m_RightVector; }
};

#endif