#include "boid.h"

Boid::Boid(float viewRad)
	:position(glm::vec3(0.0f, 0.0f, 0.0f)), velocity(glm::vec3(0.0f, 0.0f, 0.0f)), forceBuffer(glm::vec3(0.0f, 0.0f, 0.0f)), viewRadius(viewRad), color(1.0, 1.0, 1.0)
{
}

Boid::Boid(glm::vec3 startingPosition, float viewRad)
	:position(startingPosition), velocity(glm::vec3(0.0f, 0.0f, 0.0f)), forceBuffer(glm::vec3(0.0f, 0.0f, 0.0f)), viewRadius(viewRad), color(1.0, 1.0, 1.0)
{
}

Boid::Boid(glm::vec3 startingPosition, glm::vec3 startingVelocity, float viewRad)
	:position(startingPosition), velocity(startingVelocity), forceBuffer(glm::vec3(0.0f, 0.0f, 0.0f)), viewRadius(viewRad), color(1.0, 1.0, 1.0)
{
}

void Boid::AddForce(glm::vec3 force)
{
	forceBuffer += force;
}

void Boid::Move(float deltaTime)
{
	if (forceBuffer.x || forceBuffer.y || forceBuffer.z) {
		velocity += forceBuffer * deltaTime;
		forceBuffer.x = 0.0f;
		forceBuffer.y = 0.0f;
		forceBuffer.z = 0.0f;
	}
	position += velocity * deltaTime;
}
