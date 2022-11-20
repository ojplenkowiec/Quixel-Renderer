#ifndef BOID_H
#define BOID_H

#include <glm.hpp>

class Boid {
public:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 forceBuffer;

	Boid();
	Boid(glm::vec3 startingPosition);
	Boid(glm::vec3 startingPosition, glm::vec3 startingVelocity);

	void AddForce(glm::vec3 force);
	void Move(float deltaTime);
};

#endif