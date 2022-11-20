#include "boid.h"

Boid::Boid()
	:position(glm::vec3(0.0f, 0.0f, 0.0f)), velocity(glm::vec3(0.0f, 0.0f, 0.0f)), forceBuffer(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

Boid::Boid(glm::vec3 startingPosition)
	:position(startingPosition), velocity(glm::vec3(0.0f, 0.0f, 0.0f)), forceBuffer(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

Boid::Boid(glm::vec3 startingPosition, glm::vec3 startingVelocity)
	:position(startingPosition), velocity(startingVelocity), forceBuffer(glm::vec3(0.0f, 0.0f, 0.0f))
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
	}
	else {
		position += velocity * deltaTime;
	}
}
