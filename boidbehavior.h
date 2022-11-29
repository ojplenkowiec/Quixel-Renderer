#ifndef BOIDBEHAVIOR_H
#define BOIDBEHAVIOR_H

#include "boid.h"
#include "octree.h"
#include "randomranges.h"
#include "globals.h"
#include "vectorextras.h"

static void ComputeBoids(Boid** boids, unsigned int numBoids, Octree* octree) {
	float avoidanceStrength = 80.0f;
	float cohesionFactor = 1.0f;
	float alignmentFactor = 1.0f;
	
	unsigned int i = 0;
	unsigned int j = 0; // increments for speed
	
	unsigned int numberInQueryRangeIncludingSelf = 0;
	unsigned int numberInInteractionRadius = 0;
	
	glm::vec3 averageVelocities(0.0f, 0.0f, 0.0f);
	glm::vec3 averagePositions(0.0f, 0.0f, 0.0f);
	glm::vec3 otherToSelfVector(0.0f, 0.0f, 0.0f);
	
	float distance = 0.0f;
	
	std::vector<void*>* boidsInRange = new std::vector<void*>();
	
	while (i < numBoids) { // for each boid
		boidsInRange->clear();
		boids[i]->ApplyForce(RandomVec3(0.1f));
		octree->QueryCuboid(boids[i]->GetPosition() - glm::vec3(boids[i]->GetViewRadius()), boids[i]->GetPosition() + glm::vec3(boids[i]->GetViewRadius()), boidsInRange);
		numberInQueryRangeIncludingSelf = boidsInRange->size();
		if (numberInQueryRangeIncludingSelf > 1) { // not just self
			ClearVector(&averageVelocities);
			ClearVector(&averagePositions);
			numberInInteractionRadius = 0;
			j = 0;
			while (j < numberInQueryRangeIncludingSelf) {
				otherToSelfVector = boids[i]->GetPosition() - (static_cast<Boid*>(boidsInRange->at(j))->GetPosition());
				distance = GetLengthOfVector(&otherToSelfVector);
				if (distance > 0.0f) {
					if (distance < boids[i]->GetViewRadius()) {
						numberInInteractionRadius++;
					    if (distance < boids[i]->GetAvoidanceRadius()) {
							boids[i]->ApplyForce((glm::normalize(otherToSelfVector) * avoidanceStrength) / distance);
						}
						averageVelocities += static_cast<Boid*>(boidsInRange->at(j))->GetVelocity();
						averagePositions += static_cast<Boid*>(boidsInRange->at(j))->GetPosition();
					}
				}
				j++;
			}
			boids[i]->ApplyForce((averageVelocities / static_cast<float>(numberInInteractionRadius) - boids[i]->GetVelocity()) * alignmentFactor);
			boids[i]->ApplyForce((averagePositions / static_cast<float>(numberInInteractionRadius) - boids[i]->GetPosition()) * cohesionFactor);
		}
		boids[i]->Update(g_DELTA_TIME);
		i++;
	}
	delete(boidsInRange); // cleanup heap
}

#endif