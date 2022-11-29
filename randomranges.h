#ifndef RANDOMRANGES_H
#define RANDOMRANGES_H

#include <random>
#include "glm.hpp"

static int RandomInt(int min, int max) {
	return (rand() % (max - min)) + min;
}

static glm::vec3 RandomVec3(float maxMagnitude) {
	glm::vec3 direction = glm::normalize(glm::vec3((float)RandomInt(-1000, 1000) + 0.001f, (float)RandomInt(-1000, 1000) + 0.001f, (float)RandomInt(-1000, 1000) + 0.001f));
	float magnitude = (float)(RandomInt(0, 1000)) * (float)(maxMagnitude / 1000.0f);
	direction *= magnitude;
	//std::cout << direction.x << std::endl << direction.y << std::endl << direction.z << std::endl << std::endl;
	return(direction);
}

#endif 