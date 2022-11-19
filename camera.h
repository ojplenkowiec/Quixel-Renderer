#ifndef CAMERA
#define CAMERA

#include "glfw3.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera {
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

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	inline glm::vec3 GetPosition() { return c_PositionVector; }
private:
	glm::vec3 c_PositionVector;
	glm::vec3 c_DirectionVector;
	glm::vec3 c_UpVector;
	glm::vec3 c_RightVector;

	float c_Pitch; // up/down
	float c_Yaw; // left/right

	float c_Fov; // for projection matrix
	float c_AspectRatio;
	float c_Near;
	float c_Far;

	void UpdateDirectionVectors();
};

#endif