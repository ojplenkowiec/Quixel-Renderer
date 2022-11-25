#ifndef RANDOMRANGES_H
#define RANDOMRANGES_H

#include <random>

int RandomInt(int min, int max) {
	return ((float)rand() / ((float)RAND_MAX + 1.0)) * (max - min + 1) + min;
}

float RandomFloat(float min, float max) {
	return ((float)rand() / ((float)RAND_MAX + 1.0)) * (max - min + 1.0f) + min;
}

#endif