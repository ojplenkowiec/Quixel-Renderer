#ifndef BOID_H
#define BOID_H

#include <glm.hpp>
#include "vectorextras.h"

class Boid {
private:
	glm::vec3 m_Position;
	glm::vec3 m_Velocity;
	glm::vec3 m_ForceBuffer;

	float m_ViewRadius;
	float m_AvoidanceRadius;

	float m_MaxVelocity;
	float m_MinVelocity;
public:
	glm::vec3 color; // should be on linked object! boids should just have an update/get position/get direction

	Boid(glm::vec3 startingPosition, glm::vec3 startingVelocity, float viewRadius, float avoidanceRadius, float minVelocity, float maxVelocity);

	void ApplyForce(glm::vec3 force);
	void Update(float deltaTime);

	glm::vec3 GetPosition() { return m_Position; }
	glm::vec3 GetDirection() { return glm::normalize(m_Velocity); }
	glm::vec3 GetVelocity() { return m_Velocity; }

	float GetViewRadius() { return m_ViewRadius; }
	float GetAvoidanceRadius() { return m_AvoidanceRadius; }
	
	void SetPosition(glm::vec3 position);
	//inline void SetMaxVelocity(float maxVelocity) { m_MaxVelocity = maxVelocity; }
	//inline void SetMinVelocity(float minVelocity) { m_MinVelocity = minVelocity; }
};

#endif