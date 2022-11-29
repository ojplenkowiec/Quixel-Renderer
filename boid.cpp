#include "boid.h"

Boid::Boid(glm::vec3 startingPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 startingVelocity = glm::vec3(0.0f, 0.0f, 0.0f), float viewRadius = 20.0f, float avoidanceRadius = 10.0f, float minVelocity = 10.0f, float maxVelocity = 20.0f) {
	m_Position = startingPosition;
	m_Velocity = startingVelocity;
	m_ViewRadius = viewRadius;
	m_AvoidanceRadius = avoidanceRadius;
	m_MinVelocity = minVelocity;
	m_MaxVelocity = maxVelocity;
	m_ForceBuffer = glm::vec3(0.0f, 0.0f, 0.0f);

	color = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Boid::ApplyForce(glm::vec3 force)
{
	m_ForceBuffer += force;
}

void Boid::Update(float deltaTime)
{
	m_Velocity += m_ForceBuffer * deltaTime;
	float velocityMagnitude = GetLengthOfVector(&m_Velocity);
	if (velocityMagnitude > m_MaxVelocity) {
		m_Velocity *= m_MaxVelocity / velocityMagnitude;
	}
	else if (velocityMagnitude < m_MinVelocity) {
		m_Velocity *= m_MinVelocity / velocityMagnitude;
	}
	m_Position += m_Velocity * deltaTime;
	ClearVector(&m_ForceBuffer);
}

void Boid::SetPosition(glm::vec3 position)
{
	m_Position.x = (float)position.x;
	m_Position.y = (float)position.y;
	m_Position.z = (float)position.z;
}
