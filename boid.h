#ifndef BOID_H
#define BOID_H

#include <glm.hpp>

#include "octree.h"

class Boid {
public:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 forceBuffer;

	float viewRadius;

	Boid(float viewRad);
	Boid(glm::vec3 startingPosition, float viewRad);
	Boid(glm::vec3 startingPosition, glm::vec3 startingVelocity, float viewRad);

	void AddForce(glm::vec3 force);
	void Move(float deltaTime);
};

#endif