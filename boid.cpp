#include "boid.h"

Boid::Boid(float viewRad)
	:position(glm::vec3(0.0f, 0.0f, 0.0f)), velocity(glm::vec3(0.0f, 0.0f, 0.0f)), forceBuffer(glm::vec3(0.0f, 0.0f, 0.0f)), viewRadius(viewRad)
{
}

Boid::Boid(glm::vec3 startingPosition, float viewRad)
	:position(startingPosition), velocity(glm::vec3(0.0f, 0.0f, 0.0f)), forceBuffer(glm::vec3(0.0f, 0.0f, 0.0f)), viewRadius(viewRad)
{
}

Boid::Boid(glm::vec3 startingPosition, glm::vec3 startingVelocity, float viewRad)
	:position(startingPosition), velocity(startingVelocity), forceBuffer(glm::vec3(0.0f, 0.0f, 0.0f)), viewRadius(viewRad)
{
}

void Boid::AddForce(glm::vec3 force)
{
	forceBuffer += force;
}

void Boid::Move(float deltaTime)
{
	if (forceBuffer.x || forceBuffer.y || forceBuffer.z) {
		position += (velocity * deltaTime) + (0.5f * forceBuffer * deltaTime * deltaTime);
		velocity += forceBuffer * deltaTime;
		forceBuffer = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else {
		position += velocity * deltaTime;
	}
}
