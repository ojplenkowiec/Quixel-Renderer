#ifndef BOIDBEHAVIOR_H
#define BOIDBEHAVIOR_H

#include "boid.h"
#include "octree.h"
#include "randomranges.h"
#include "globals.h"

void ComputeBoids(Boid** boids, unsigned int numBoids, Octree* octree) {
	float avoianceRadiusSquared = 300.0f;
	float avoidanceStrength = 80.0f;

	unsigned int i{};
	unsigned int j{}; // increments for speed

	unsigned int numberInQueryRangeIncludingSelf{};
	unsigned int numberInInteractionRadius{};

	glm::vec3 averageVelocities{};
	glm::vec3 averagePositions{};
	glm::vec3 otherToSelfVector{};

	float distanceSquared{};

	std::vector<void*>* boidsInRange = new std::vector<void*>{};

	while (i < numBoids) { // for each boid
		boids[i]->forceBuffer.x += RandomFloat(-1000, 1000) / 1000.0f;
		boids[i]->forceBuffer.y += RandomFloat(-1000, 1000) / 1000.0f;
		boids[i]->forceBuffer.z += RandomFloat(-1000, 1000) / 1000.0f;

		boidsInRange->clear();
		octree->QueryCuboid(boids[i]->position - glm::vec3(boids[i]->viewRadius, boids[i]->viewRadius, boids[i]->viewRadius), boids[i]->position + glm::vec3(boids[i]->viewRadius, boids[i]->viewRadius, boids[i]->viewRadius), boidsInRange);

		numberInQueryRangeIncludingSelf = boidsInRange->size();

		if (numberInQueryRangeIncludingSelf > 1) { // not just self
			averageVelocities.x = 0.0f;
			averageVelocities.y = 0.0f;
			averageVelocities.z = 0.0f;

			averagePositions.x = 0.0f;
			averagePositions.y = 0.0f;
			averagePositions.z = 0.0f;

			numberInInteractionRadius = 0;

			j = 0;
			while (j < numberInQueryRangeIncludingSelf) {
				otherToSelfVector = boids[i]->position - (static_cast<Boid*>(boidsInRange->at(j))->position);
				distanceSquared = (otherToSelfVector.x * otherToSelfVector.x) + (otherToSelfVector.y * otherToSelfVector.y) + (otherToSelfVector.z * otherToSelfVector.z);
				if (distanceSquared != 0.0f) {
					if (distanceSquared < boids[i]->viewRadius * boids[i]->viewRadius) {
						numberInInteractionRadius++;
						if (distanceSquared < avoianceRadiusSquared) {
							boids[i]->AddForce((otherToSelfVector * avoidanceStrength) / distanceSquared);
						}
						averageVelocities += static_cast<Boid*>(boidsInRange->at(j))->velocity;
						averagePositions += static_cast<Boid*>(boidsInRange->at(j))->position;
					}
				}
				j++;
			}
			boids[i]->AddForce((averageVelocities / static_cast<float>(numberInInteractionRadius) - boids[i]->velocity) * 2.0f);
			boids[i]->AddForce((averagePositions / static_cast<float>(numberInInteractionRadius) - boids[i]->position) * 1.0f);
		}
		if (boids[i]->velocity.x || boids[i]->velocity.y || boids[i]->velocity.z) {
			boids[i]->velocity *= 25.0f / glm::length(boids[i]->velocity);
		}
		else {
			boids[i]->velocity.x += RandomFloat(-1000, 1000) / 100.0f;
			boids[i]->velocity.y += RandomFloat(-1000, 1000) / 100.0f;
			boids[i]->velocity.z += RandomFloat(-1000, 1000) / 100.0f;
		}
		boids[i]->Move(g_DELTA_TIME);
		i++;
	}
	delete(boidsInRange); // cleanup heap
}

#endif