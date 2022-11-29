#ifndef VECTOREXTRAS_H
#define VECTOREXTRAS_H

#include "glm.hpp"

static void ClearVector(glm::vec3* vector) {
	vector->x = 0.0f;
	vector->y = 0.0f;
	vector->z = 0.0f;
}

static float GetLengthOfVector(glm::vec3* vector) {
	return std::sqrt(float(vector->x) * float(vector->x) + float(vector->y) * float(vector->y) + float(vector->z) * float(vector->z));
}

#endif
