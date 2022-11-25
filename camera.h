#ifndef CAMERA_H
#define CAMERA_H

#include "glfw3.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera { // camera should have a target window?
private:
	glm::vec3 m_PositionVector;

	glm::vec3 m_UpVector;
	glm::vec3 m_RightVector;
	glm::vec3 m_ForwardsVector;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	bool m_ViewMatrixNeedsUpdate;
	bool m_ProjectionMatrixNeedsUpdate;

	float c_Pitch; // up/down
	float c_Yaw; // left/right

	float m_FieldOfView;
	float m_AspectRatio;
	float m_NearClipDistance;
	float m_FarClipDistance;

	void UpdateDirectionVectors();

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
public:
	Camera(float aspectRatio, float fov, float nearDistance, float farDistance);

	void MoveForwards(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);
	void Rotate(float dPitch, float dYaw);

	void TranslateForwards(float distance);
	void TranslateRight(float distance);
	void TranslateUp(float distance);
	void Translate(glm::vec3 translationVector);

	inline void UpdateAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

	void FocusOn(glm::vec3 position); // ACTUALLY FINISH THIS! maybe too abstract...

	glm::vec3 GetRay(float mouse_x, float mouse_y, float windowWidth, float windowHeight); // idk about this at all its so weird... maybe should be created in scene?

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	inline glm::vec3 GetPosition() { return m_PositionVector; }
	inline glm::vec3 GetUpVector() { return m_UpVector; }
	inline glm::vec3 GetRightVector() { return m_RightVector; }
	inline glm::vec3 GetForwardsVector() { return m_ForwardsVector; }
};

#endif