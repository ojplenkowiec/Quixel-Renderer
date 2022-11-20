#ifndef OCTREE_H
#define OCTREE_H

#include <glm.hpp>
#include <vector>

struct Node {
	glm::vec3* position;
	void* dataPointer;
};

class Octree {
private:
	glm::vec3 maxPoint; // front, right, top 
	glm::vec3 minPoint; // back, left, bottom

	glm::vec3 midPoint;

	void* storedData[8]{}; // min bound clockwise up
	uint8_t branchState; // same as above

	Node* temp;

	uint8_t maxDepth;
public:
	Octree(glm::vec3 minPointVector, glm::vec3 maxPointVector, uint8_t depthLimit);
	~Octree();

	void PushData(glm::vec3* position, void* dataPointer);
	void PushData(Node* node);
	void GetVertices(std::vector<glm::vec3>* vectorPointer);

	void QueryCuboid(glm::vec3 cuboidMin, glm::vec3 cuboidMax, std::vector<void*>* vectorResultPointer);
};

#endif