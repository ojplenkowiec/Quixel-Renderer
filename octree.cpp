#include "octree.h"

#include <iostream>

Octree::Octree(glm::vec3 minPointVector, glm::vec3 maxPointVector, uint8_t depthLimit)
	:branchState(0b00000000), minPoint(minPointVector), maxPoint(maxPointVector), maxDepth(depthLimit)
{
	midPoint = (minPoint + maxPoint) / 2.0f;

	storedData[0] = nullptr;
	storedData[1] = nullptr;
	storedData[2] = nullptr;
	storedData[3] = nullptr;
	storedData[4] = nullptr;
	storedData[5] = nullptr;
	storedData[6] = nullptr;
	storedData[7] = nullptr;

	temp = nullptr;
}

Octree::~Octree()
{
	if (branchState & 0b00000001) {
		delete(static_cast<Octree*>(storedData[7]));
	}
	else {
		if (storedData[7]) {
			delete(static_cast<Node*>(storedData[7]));
		}
	}
	if (branchState & 0b00000010) {
		delete(static_cast<Octree*>(storedData[6]));
	}
	else {
		if (storedData[6]) {
			delete(static_cast<Node*>(storedData[6]));
		}
	}
	if (branchState & 0b00000100) {
		delete(static_cast<Octree*>(storedData[5]));
	}
	else {
		if (storedData[5]) {
			delete(static_cast<Node*>(storedData[5]));
		}
	}
	if (branchState & 0b00001000) {
		delete(static_cast<Octree*>(storedData[4]));
	}
	else {
		if (storedData[4]) {
			delete(static_cast<Node*>(storedData[4]));
		}
	}
	if (branchState & 0b00010000) {
		delete(static_cast<Octree*>(storedData[3]));
	}
	else {
		if (storedData[3]) {
			delete(static_cast<Node*>(storedData[3]));
		}
	}
	if (branchState & 0b00100000) {
		delete(static_cast<Octree*>(storedData[2]));
	}
	else {
		if (storedData[2]) {
			delete(static_cast<Node*>(storedData[2]));
		}
	}
	if (branchState & 0b01000000) {
		delete(static_cast<Octree*>(storedData[1]));
	}
	else {
		if (storedData[1]) {
			delete(static_cast<Node*>(storedData[1]));
		}
	}
	if (branchState & 0b10000000) {
		delete(static_cast<Octree*>(storedData[0]));
	}
	else {
		if (storedData[0]) {
			delete(static_cast<Node*>(storedData[0]));
		}
	}
}

void Octree::PushData(glm::vec3* position, void* dataPointer)
{
	if (position->x >= midPoint.x) {
		if (position->y >= midPoint.y) {
			if (position->z >= midPoint.z) { // +x +y +z [6]
				if (branchState & 0b00000010) {
					static_cast<Octree*>(storedData[6])->PushData(position, dataPointer);
				}
				else {
					if (storedData[6] == nullptr) {
						storedData[6] = new Node{ position, dataPointer };
					}
					else if(maxDepth > 0) {
						temp = static_cast<Node*>(storedData[6]);

						storedData[6] = new Octree(midPoint, maxPoint, maxDepth - 1);
						branchState = branchState | 0b00000010;

						static_cast<Octree*>(storedData[6])->PushData(temp);
						static_cast<Octree*>(storedData[6])->PushData(position, dataPointer);
					}
				}
			}
			else { // +x +y -z [5]
				if (branchState & 0b00000100) {
					static_cast<Octree*>(storedData[5])->PushData(position, dataPointer);
				}
				else {
					if (storedData[5] == nullptr) {
						storedData[5] = new Node{ position, dataPointer };
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[5]);

						storedData[5] = new Octree(glm::vec3(midPoint.x, midPoint.y, minPoint.z), glm::vec3(maxPoint.x, maxPoint.y, midPoint.z), maxDepth - 1);
						branchState = branchState | 0b00000100;

						static_cast<Octree*>(storedData[5])->PushData(temp);
						static_cast<Octree*>(storedData[5])->PushData(position, dataPointer);
					}
				}
			}
		}
		else {
			if (position->z >= midPoint.z) { // +x -y +z [2]
				if (branchState & 0b00100000) {
					static_cast<Octree*>(storedData[2])->PushData(position, dataPointer);
				}
				else {
					if (storedData[2] == nullptr) {
						storedData[2] = new Node{ position, dataPointer };
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[2]);

						storedData[2] = new Octree(glm::vec3(midPoint.x, minPoint.y, midPoint.z), glm::vec3(maxPoint.x, midPoint.y, maxPoint.z), maxDepth - 1);
						branchState = branchState | 0b00100000;

						static_cast<Octree*>(storedData[2])->PushData(temp);
						static_cast<Octree*>(storedData[2])->PushData(position, dataPointer);
					}
				}
			}
			else { // +x -y -z [1]
				if (branchState & 0b01000000) {
					static_cast<Octree*>(storedData[1])->PushData(position, dataPointer);
				}
				else {
					if (storedData[1] == nullptr) {
						storedData[1] = new Node{ position, dataPointer };
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[1]);

						storedData[1] = new Octree(glm::vec3(midPoint.x, minPoint.y, minPoint.z), glm::vec3(maxPoint.x, midPoint.y, midPoint.z), maxDepth - 1);
						branchState = branchState | 0b01000000;

						static_cast<Octree*>(storedData[1])->PushData(temp);
						static_cast<Octree*>(storedData[1])->PushData(position, dataPointer);
					}
				}
			}
		}
	}
	else {
		if (position->y >= midPoint.y) {
			if (position->z >= midPoint.z) { // -x +y +z [7]
				if (branchState & 0b00000001) {
					static_cast<Octree*>(storedData[7])->PushData(position, dataPointer);
				}
				else {
					if (storedData[7] == nullptr) {
						storedData[7] = new Node{ position, dataPointer };
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[7]);

						storedData[7] = new Octree(glm::vec3(minPoint.x, midPoint.y, midPoint.z), glm::vec3(midPoint.x, maxPoint.y, maxPoint.z), maxDepth - 1);
						branchState = branchState | 0b00000001;

						static_cast<Octree*>(storedData[7])->PushData(temp);
						static_cast<Octree*>(storedData[7])->PushData(position, dataPointer);
					}
				}
			}
			else { // -x +y -z [4]
				if (branchState & 0b00001000) {
					static_cast<Octree*>(storedData[4])->PushData(position, dataPointer);
				}
				else {
					if (storedData[4] == nullptr) {
						storedData[4] = new Node{ position, dataPointer };
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[4]);

						storedData[4] = new Octree(glm::vec3(minPoint.x, midPoint.y, minPoint.z), glm::vec3(midPoint.x, maxPoint.y, midPoint.z), maxDepth - 1);
						branchState = branchState | 0b00001000;

						static_cast<Octree*>(storedData[4])->PushData(temp);
						static_cast<Octree*>(storedData[4])->PushData(position, dataPointer);
					}
				}
			}
		}
		else {
			if (position->z >= midPoint.z) { // -x -y +z [3]
				if (branchState & 0b00010000) {
					static_cast<Octree*>(storedData[3])->PushData(position, dataPointer);
				}
				else {
					if (storedData[3] == nullptr) {
						storedData[3] = new Node{ position, dataPointer };
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[3]);

						storedData[3] = new Octree(glm::vec3(minPoint.x, minPoint.y, midPoint.z), glm::vec3(midPoint.x, midPoint.y, maxPoint.z), maxDepth - 1);
						branchState = branchState | 0b00010000;

						static_cast<Octree*>(storedData[3])->PushData(temp);
						static_cast<Octree*>(storedData[3])->PushData(position, dataPointer);
					}
				}
			}
			else { // -x -y -z [0]
				if (branchState & 0b10000000) {
					static_cast<Octree*>(storedData[0])->PushData(position, dataPointer);
				}
				else {
					if (storedData[0] == nullptr) {
						storedData[0] = new Node{ position, dataPointer };
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[0]);

						storedData[0] = new Octree(minPoint, midPoint, maxDepth - 1);
						branchState = branchState | 0b10000000;

						static_cast<Octree*>(storedData[0])->PushData(temp);
						static_cast<Octree*>(storedData[0])->PushData(position, dataPointer);
					}
				}
			}
		}
	}
}

void Octree::PushData(Node* node)
{
	if (node->position->x >= midPoint.x) {
		if (node->position->y >= midPoint.y) {
			if (node->position->z >= midPoint.z) { // +x +y +z [6]
				if (branchState & 0b00000010) {
					static_cast<Octree*>(storedData[6])->PushData(node);
				}
				else {
					if (storedData[6] == nullptr) {
						storedData[6] = node;
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[6]);

						storedData[6] = new Octree(midPoint, maxPoint, maxDepth - 1);
						branchState = branchState | 0b00000010;

						static_cast<Octree*>(storedData[6])->PushData(temp);
						static_cast<Octree*>(storedData[6])->PushData(node);
					}
				}
			}
			else { // +x +y -z [5]
				if (branchState & 0b00000100) {
					static_cast<Octree*>(storedData[5])->PushData(node);
				}
				else {
					if (storedData[5] == nullptr) {
						storedData[5] = node;
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[5]);

						storedData[5] = new Octree(glm::vec3(midPoint.x, midPoint.y, minPoint.z), glm::vec3(maxPoint.x, maxPoint.y, midPoint.z), maxDepth - 1);
						branchState = branchState | 0b00000100;

						static_cast<Octree*>(storedData[5])->PushData(temp);
						static_cast<Octree*>(storedData[5])->PushData(node);
					}
				}
			}
		}
		else {
			if (node->position->z >= midPoint.z) { // +x -y +z [2]
				if (branchState & 0b00100000) {
					static_cast<Octree*>(storedData[2])->PushData(node);
				}
				else {
					if (storedData[2] == nullptr) {
						storedData[2] = node;
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[2]);

						storedData[2] = new Octree(glm::vec3(midPoint.x, minPoint.y, midPoint.z), glm::vec3(maxPoint.x, midPoint.y, maxPoint.z), maxDepth - 1);
						branchState = branchState | 0b00100000;

						static_cast<Octree*>(storedData[2])->PushData(temp);
						static_cast<Octree*>(storedData[2])->PushData(node);
					}
				}
			}
			else { // +x -y -z [1]
				if (branchState & 0b01000000) {
					static_cast<Octree*>(storedData[1])->PushData(node);
				}
				else {
					if (storedData[1] == nullptr) {
						storedData[1] = node;
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[1]);

						storedData[1] = new Octree(glm::vec3(midPoint.x, minPoint.y, minPoint.z), glm::vec3(maxPoint.x, midPoint.y, midPoint.z), maxDepth - 1);
						branchState = branchState | 0b01000000;

						static_cast<Octree*>(storedData[1])->PushData(temp);
						static_cast<Octree*>(storedData[1])->PushData(node);
					}
				}
			}
		}
	}
	else {
		if (node->position->y >= midPoint.y) {
			if (node->position->z >= midPoint.z) { // -x +y +z [7]
				if (branchState & 0b00000001) {
					static_cast<Octree*>(storedData[7])->PushData(node);
				}
				else {
					if (storedData[7] == nullptr) {
						storedData[7] = node;
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[7]);

						storedData[7] = new Octree(glm::vec3(minPoint.x, midPoint.y, midPoint.z), glm::vec3(midPoint.x, maxPoint.y, maxPoint.z), maxDepth - 1);
						branchState = branchState | 0b00000001;

						static_cast<Octree*>(storedData[7])->PushData(temp);
						static_cast<Octree*>(storedData[7])->PushData(node);
					}
				}
			}
			else { // -x +y -z [4]
				if (branchState & 0b00001000) {
					static_cast<Octree*>(storedData[4])->PushData(node);
				}
				else {
					if (storedData[4] == nullptr) {
						storedData[4] = node;
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[4]);

						storedData[4] = new Octree(glm::vec3(minPoint.x, midPoint.y, minPoint.z), glm::vec3(midPoint.x, maxPoint.y, midPoint.z), maxDepth - 1);
						branchState = branchState | 0b00001000;

						static_cast<Octree*>(storedData[4])->PushData(temp);
						static_cast<Octree*>(storedData[4])->PushData(node);
					}
				}
			}
		}
		else {
			if (node->position->z >= midPoint.z) { // -x -y +z [3]
				if (branchState & 0b00010000) {
					static_cast<Octree*>(storedData[3])->PushData(node);
				}
				else {
					if (storedData[3] == nullptr) {
						storedData[3] = node;
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[3]);

						storedData[3] = new Octree(glm::vec3(minPoint.x, minPoint.y, midPoint.z), glm::vec3(midPoint.x, midPoint.y, maxPoint.z), maxDepth - 1);
						branchState = branchState | 0b00010000;

						static_cast<Octree*>(storedData[3])->PushData(temp);
						static_cast<Octree*>(storedData[3])->PushData(node);
					}
				}
			}
			else { // -x -y -z [0]
				if (branchState & 0b10000000) {
					static_cast<Octree*>(storedData[0])->PushData(node);
				}
				else {
					if (storedData[0] == nullptr) {
						storedData[0] = node;
					}
					else if (maxDepth > 0) {
						temp = static_cast<Node*>(storedData[0]);

						storedData[0] = new Octree(minPoint, midPoint, maxDepth - 1);
						branchState = branchState | 0b10000000;

						static_cast<Octree*>(storedData[0])->PushData(temp);
						static_cast<Octree*>(storedData[0])->PushData(node);
					}
				}
			}
		}
	}
}

void Octree::GetVertices(std::vector<glm::vec3>* vectorPointer)
{
	if (!(branchState & 0b11111111)) {
		glm::vec3 BLB = minPoint;

		glm::vec3 BRB = glm::vec3(maxPoint.x, minPoint.y, minPoint.z);
		glm::vec3 BRF = glm::vec3(maxPoint.x, minPoint.y, maxPoint.z);
		glm::vec3 BLF = glm::vec3(minPoint.x, minPoint.y, maxPoint.z);
		glm::vec3 TLB = glm::vec3(minPoint.x, maxPoint.y, minPoint.z);
		glm::vec3 TRB = glm::vec3(maxPoint.x, maxPoint.y, minPoint.z);

		glm::vec3 TRF = maxPoint;

		glm::vec3 TLF = glm::vec3(minPoint.x, maxPoint.y, maxPoint.z);

		vectorPointer->push_back(BLB);
		vectorPointer->push_back(BRB);

		vectorPointer->push_back(BLB);
		vectorPointer->push_back(TLB);

		vectorPointer->push_back(BLB);
		vectorPointer->push_back(BLF);

		vectorPointer->push_back(TRF);
		vectorPointer->push_back(TRB);

		vectorPointer->push_back(TRF);
		vectorPointer->push_back(TLF);

		vectorPointer->push_back(TRF);
		vectorPointer->push_back(BRF);

		vectorPointer->push_back(BRB);
		vectorPointer->push_back(TRB);

		vectorPointer->push_back(TRB);
		vectorPointer->push_back(TLB);

		vectorPointer->push_back(TLB);
		vectorPointer->push_back(TLF);

		vectorPointer->push_back(TLF);
		vectorPointer->push_back(BLF);

		vectorPointer->push_back(BLF);
		vectorPointer->push_back(BRF);

		vectorPointer->push_back(BRF);
		vectorPointer->push_back(BRB); // GL LINES FORMAT
	}
	else {
		if (branchState & 0b00000001) {
			static_cast<Octree*>(storedData[7])->GetVertices(vectorPointer);
		}
		if (branchState & 0b00000010) {
			static_cast<Octree*>(storedData[6])->GetVertices(vectorPointer);
		}
		if (branchState & 0b00000100) {
			static_cast<Octree*>(storedData[5])->GetVertices(vectorPointer);
		}
		if (branchState & 0b00001000) {
			static_cast<Octree*>(storedData[4])->GetVertices(vectorPointer);
		}
		if (branchState & 0b00010000) {
			static_cast<Octree*>(storedData[3])->GetVertices(vectorPointer);
		}
		if (branchState & 0b00100000) {
			static_cast<Octree*>(storedData[2])->GetVertices(vectorPointer);
		}
		if (branchState & 0b01000000) {
			static_cast<Octree*>(storedData[1])->GetVertices(vectorPointer);
		}
		if (branchState & 0b10000000) {
			static_cast<Octree*>(storedData[0])->GetVertices(vectorPointer);
		}
	}

}

bool DoesIntersect(glm::vec3* aMin, glm::vec3* aMax, glm::vec3 bMin, glm::vec3 bMax) {
	return(aMin->x <= bMax.x && aMax->x >= bMin.x && aMin->y <= bMax.y && aMax->y >= bMin.y && aMin->z <= bMax.z && aMax->z >= bMin.z);
} // FUNCTION FOR CHECKING INTERSECTION

void Octree::QueryCuboid(glm::vec3 cuboidMin, glm::vec3 cuboidMax, std::vector<void*>* vectorResultPointer)
{
	if (storedData[0]) {
		if (DoesIntersect(&cuboidMin, &cuboidMax, minPoint, midPoint)) {
			if (branchState & 0b10000000) {
				static_cast<Octree*>(storedData[0])->QueryCuboid(cuboidMin, cuboidMax, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
			}
		}
	}
	if (storedData[1]) {
		if (DoesIntersect(&cuboidMin, &cuboidMax, glm::vec3(midPoint.x, minPoint.y, minPoint.z), glm::vec3(maxPoint.x, midPoint.y, midPoint.z))) {
			if (branchState & 0b01000000) {
				static_cast<Octree*>(storedData[1])->QueryCuboid(cuboidMin, cuboidMax, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
			}
		}
	}
	if (storedData[2]) {
		if (DoesIntersect(&cuboidMin, &cuboidMax, glm::vec3(midPoint.x, minPoint.y, midPoint.z), glm::vec3(maxPoint.x, midPoint.y, maxPoint.z))) {
			if (branchState & 0b00100000) {
				static_cast<Octree*>(storedData[2])->QueryCuboid(cuboidMin, cuboidMax, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
			}
		}
	}
	if (storedData[3]) {
		if (DoesIntersect(&cuboidMin, &cuboidMax, glm::vec3(minPoint.x, minPoint.y, midPoint.z), glm::vec3(midPoint.x, midPoint.y, maxPoint.z))) {
			if (branchState & 0b00010000) {
				static_cast<Octree*>(storedData[3])->QueryCuboid(cuboidMin, cuboidMax, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
			}
		}
	}
	if (storedData[4]) {
		if (DoesIntersect(&cuboidMin, &cuboidMax, glm::vec3(minPoint.x, midPoint.y, minPoint.z), glm::vec3(midPoint.x, maxPoint.y, midPoint.z))) {
			if (branchState & 0b00001000) {
				static_cast<Octree*>(storedData[4])->QueryCuboid(cuboidMin, cuboidMax, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
			}
		}
	}
	if (storedData[5]) {
		if (DoesIntersect(&cuboidMin, &cuboidMax, glm::vec3(midPoint.x, midPoint.y, minPoint.z), glm::vec3(maxPoint.x, maxPoint.y, midPoint.z))) {
			if (branchState & 0b00000100) {
				static_cast<Octree*>(storedData[5])->QueryCuboid(cuboidMin, cuboidMax, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
			}
		}
	}
	if (storedData[6]) {
		if (DoesIntersect(&cuboidMin, &cuboidMax, midPoint, maxPoint)) {
			if (branchState & 0b00000010) {
				static_cast<Octree*>(storedData[6])->QueryCuboid(cuboidMin, cuboidMax, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
			}
		}
	}
	if (storedData[7]) {
		if (DoesIntersect(&cuboidMin, &cuboidMax, glm::vec3(minPoint.x, midPoint.y, midPoint.z), glm::vec3(midPoint.x, maxPoint.y, maxPoint.z))) {
			if (branchState & 0b00000001) {
				static_cast<Octree*>(storedData[7])->QueryCuboid(cuboidMin, cuboidMax, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
			}
		}
	}
}

void Octree::QueryRay(glm::vec3 rayCastLocation, glm::vec3 rayDirection, std::vector<void*>* vectorResultPointer)
{
	// for each
	// check if data...
	// if data then find closest point on cube to A
	// calculate the inverse matrices for triangles of adjacent planes, check all to see if any intersect as going along as if they do we can stop
	// if intersect, see if branched
	// if branched, query ray of branch
	// if not branched, retrieve data

	glm::vec3 ray = rayDirection - rayCastLocation;

	glm::vec3 raysToPoints[8]{};

	glm::vec3 localMinPoint{};
	glm::vec3 localMaxPoint{};

	glm::vec3 result{};

	if (storedData[0]) {
		localMinPoint = minPoint;
		localMaxPoint = midPoint;

		raysToPoints[0] = localMinPoint - rayCastLocation;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - rayCastLocation;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
			if (branchState & 0b10000000) {
				static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
				if (branchState & 0b10000000) {
					static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
				}
				else {
					vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - rayCastLocation;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
					if (branchState & 0b10000000) {
						static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
					}
					else {
						vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
						if (branchState & 0b10000000) {
							static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
						}
						else {
							vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
							if (branchState & 0b10000000) {
								static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
							}
							else {
								vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - rayCastLocation;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
								if (branchState & 0b10000000) {
									static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
								}
								else {
									vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
									if (branchState & 0b10000000) {
										static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
									}
									else {
										vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
										if (branchState & 0b10000000) {
											static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
										}
										else {
											vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
											if (branchState & 0b10000000) {
												static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
											}
											else {
												vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
												if (branchState & 0b10000000) {
													static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
												}
												else {
													vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
													if (branchState & 0b10000000) {
														static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
													}
													else {
														vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
														if (branchState & 0b10000000) {
															static_cast<Octree*>(storedData[0])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
														}
														else {
															vectorResultPointer->push_back(static_cast<Node*>(storedData[0])->dataPointer);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[1]) {
		localMinPoint = glm::vec3(midPoint.x, minPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, midPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - rayCastLocation;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - rayCastLocation;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
			if (branchState & 0b01000000) {
				static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
				if (branchState & 0b01000000) {
					static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
				}
				else {
					vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - rayCastLocation;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
					if (branchState & 0b01000000) {
						static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
					}
					else {
						vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
						if (branchState & 0b01000000) {
							static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
						}
						else {
							vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
							if (branchState & 0b01000000) {
								static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
							}
							else {
								vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - rayCastLocation;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
								if (branchState & 0b01000000) {
									static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
								}
								else {
									vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
									if (branchState & 0b01000000) {
										static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
									}
									else {
										vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
										if (branchState & 0b01000000) {
											static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
										}
										else {
											vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
											if (branchState & 0b01000000) {
												static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
											}
											else {
												vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
												if (branchState & 0b01000000) {
													static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
												}
												else {
													vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
													if (branchState & 0b01000000) {
														static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
													}
													else {
														vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
														if (branchState & 0b01000000) {
															static_cast<Octree*>(storedData[1])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
														}
														else {
															vectorResultPointer->push_back(static_cast<Node*>(storedData[1])->dataPointer);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[2]) {
		localMinPoint = glm::vec3(midPoint.x, minPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, midPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - rayCastLocation;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - rayCastLocation;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
			if (branchState & 0b00100000) {
				static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
				if (branchState & 0b00100000) {
					static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
				}
				else {
					vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - rayCastLocation;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
					if (branchState & 0b00100000) {
						static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
					}
					else {
						vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
						if (branchState & 0b00100000) {
							static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
						}
						else {
							vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
							if (branchState & 0b00100000) {
								static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
							}
							else {
								vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - rayCastLocation;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
								if (branchState & 0b00100000) {
									static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
								}
								else {
									vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
									if (branchState & 0b00100000) {
										static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
									}
									else {
										vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
										if (branchState & 0b00100000) {
											static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
										}
										else {
											vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
											if (branchState & 0b00100000) {
												static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
											}
											else {
												vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
												if (branchState & 0b00100000) {
													static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
												}
												else {
													vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
													if (branchState & 0b00100000) {
														static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
													}
													else {
														vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
														if (branchState & 0b00100000) {
															static_cast<Octree*>(storedData[2])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
														}
														else {
															vectorResultPointer->push_back(static_cast<Node*>(storedData[2])->dataPointer);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[3]) {
		localMinPoint = glm::vec3(minPoint.x, minPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, midPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - rayCastLocation;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - rayCastLocation;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
			if (branchState & 0b00010000) {
				static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
				if (branchState & 0b00010000) {
					static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
				}
				else {
					vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - rayCastLocation;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
					if (branchState & 0b00010000) {
						static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
					}
					else {
						vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
						if (branchState & 0b00010000) {
							static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
						}
						else {
							vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
							if (branchState & 0b00010000) {
								static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
							}
							else {
								vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - rayCastLocation;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
								if (branchState & 0b00010000) {
									static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
								}
								else {
									vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
									if (branchState & 0b00010000) {
										static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
									}
									else {
										vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
										if (branchState & 0b00010000) {
											static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
										}
										else {
											vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
											if (branchState & 0b00010000) {
												static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
											}
											else {
												vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
												if (branchState & 0b00010000) {
													static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
												}
												else {
													vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
													if (branchState & 0b00010000) {
														static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
													}
													else {
														vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
														if (branchState & 0b00010000) {
															static_cast<Octree*>(storedData[3])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
														}
														else {
															vectorResultPointer->push_back(static_cast<Node*>(storedData[3])->dataPointer);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[4]) {
		localMinPoint = glm::vec3(minPoint.x, midPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, maxPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - rayCastLocation;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - rayCastLocation;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
			if (branchState & 0b00001000) {
				static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
				if (branchState & 0b00001000) {
					static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
				}
				else {
					vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - rayCastLocation;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
					if (branchState & 0b00001000) {
						static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
					}
					else {
						vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
						if (branchState & 0b00001000) {
							static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
						}
						else {
							vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
							if (branchState & 0b00001000) {
								static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
							}
							else {
								vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - rayCastLocation;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
								if (branchState & 0b00001000) {
									static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
								}
								else {
									vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
									if (branchState & 0b00001000) {
										static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
									}
									else {
										vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
										if (branchState & 0b00001000) {
											static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
										}
										else {
											vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
											if (branchState & 0b00001000) {
												static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
											}
											else {
												vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
												if (branchState & 0b00001000) {
													static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
												}
												else {
													vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
													if (branchState & 0b00001000) {
														static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
													}
													else {
														vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
														if (branchState & 0b00001000) {
															static_cast<Octree*>(storedData[4])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
														}
														else {
															vectorResultPointer->push_back(static_cast<Node*>(storedData[4])->dataPointer);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[5]) {
		localMinPoint = glm::vec3(midPoint.x, midPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, maxPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - rayCastLocation;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - rayCastLocation;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
			if (branchState & 0b00000100) {
				static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
				if (branchState & 0b00000100) {
					static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
				}
				else {
					vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - rayCastLocation;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
					if (branchState & 0b00000100) {
						static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
					}
					else {
						vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
						if (branchState & 0b00000100) {
							static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
						}
						else {
							vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
							if (branchState & 0b00000100) {
								static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
							}
							else {
								vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - rayCastLocation;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
								if (branchState & 0b00000100) {
									static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
								}
								else {
									vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
									if (branchState & 0b00000100) {
										static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
									}
									else {
										vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
										if (branchState & 0b00000100) {
											static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
										}
										else {
											vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
											if (branchState & 0b00000100) {
												static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
											}
											else {
												vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
												if (branchState & 0b00000100) {
													static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
												}
												else {
													vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
													if (branchState & 0b00000100) {
														static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
													}
													else {
														vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
														if (branchState & 0b00000100) {
															static_cast<Octree*>(storedData[5])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
														}
														else {
															vectorResultPointer->push_back(static_cast<Node*>(storedData[5])->dataPointer);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[6]) {
		localMinPoint = midPoint;
		localMaxPoint = maxPoint;

		raysToPoints[0] = localMinPoint - rayCastLocation;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - rayCastLocation;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
			if (branchState & 0b00000010) {
				static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
				if (branchState & 0b00000010) {
					static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
				}
				else {
					vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - rayCastLocation;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
					if (branchState & 0b00000010) {
						static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
					}
					else {
						vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
						if (branchState & 0b00000010) {
							static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
						}
						else {
							vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
							if (branchState & 0b00000010) {
								static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
							}
							else {
								vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - rayCastLocation;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
								if (branchState & 0b00000010) {
									static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
								}
								else {
									vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
									if (branchState & 0b00000010) {
										static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
									}
									else {
										vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
										if (branchState & 0b00000010) {
											static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
										}
										else {
											vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
											if (branchState & 0b00000010) {
												static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
											}
											else {
												vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
												if (branchState & 0b00000010) {
													static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
												}
												else {
													vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
													if (branchState & 0b00000010) {
														static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
													}
													else {
														vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
														if (branchState & 0b00000010) {
															static_cast<Octree*>(storedData[6])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
														}
														else {
															vectorResultPointer->push_back(static_cast<Node*>(storedData[6])->dataPointer);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[7]) {
		localMinPoint = glm::vec3(minPoint.x, midPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, maxPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - rayCastLocation;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - rayCastLocation;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
			if (branchState & 0b00000001) {
				static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
			}
			else {
				vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - rayCastLocation;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
				if (branchState & 0b00000001) {
					static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
				}
				else {
					vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - rayCastLocation;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
					if (branchState & 0b00000001) {
						static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
					}
					else {
						vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
						if (branchState & 0b00000001) {
							static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
						}
						else {
							vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
							if (branchState & 0b00000001) {
								static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
							}
							else {
								vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - rayCastLocation;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
								if (branchState & 0b00000001) {
									static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
								}
								else {
									vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
									if (branchState & 0b00000001) {
										static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
									}
									else {
										vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - rayCastLocation;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
										if (branchState & 0b00000001) {
											static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
										}
										else {
											vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
											if (branchState & 0b00000001) {
												static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
											}
											else {
												vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
												if (branchState & 0b00000001) {
													static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
												}
												else {
													vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
													if (branchState & 0b00000001) {
														static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
													}
													else {
														vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x > 0.0f && result.y > 0.0f && result.z > 0.0f) {
														if (branchState & 0b00000001) {
															static_cast<Octree*>(storedData[7])->QueryRay(rayCastLocation, rayDirection, vectorResultPointer);
														}
														else {
															vectorResultPointer->push_back(static_cast<Node*>(storedData[7])->dataPointer);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void* Octree::RayCastClosest(glm::vec3 origin, glm::vec3 rayDirection, float maxDistanceFromRay)
{
	glm::vec3 ray = rayDirection - origin;

	glm::vec3 raysToPoints[8]{};

	glm::vec3 localMinPoint{};
	glm::vec3 localMaxPoint{};

	glm::vec3 result{};

	std::vector<Node*>* rayCollisionNodes = new std::vector<Node*>{};

	if (storedData[0] != nullptr) {
		localMinPoint = minPoint;
		localMaxPoint = midPoint;

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin; 
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b10000000) {
				static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes); 
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b10000000) {
					static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b10000000) {
						static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b10000000) {
							static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b10000000) {
								static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b10000000) {
									static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b10000000) {
										static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b10000000) {
											static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b10000000) {
												static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b10000000) {
													static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b10000000) {
														static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b10000000) {
															static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[1] != nullptr) {
		localMinPoint = glm::vec3(midPoint.x, minPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, midPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b01000000) {
				static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b01000000) {
					static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b01000000) {
						static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b01000000) {
							static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b01000000) {
								static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b01000000) {
									static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b01000000) {
										static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b01000000) {
											static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b01000000) {
												static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b01000000) {
													static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b01000000) {
														static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b01000000) {
															static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[2] != nullptr) {
		localMinPoint = glm::vec3(midPoint.x, minPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, midPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00100000) {
				static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00100000) {
					static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00100000) {
						static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00100000) {
							static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00100000) {
								static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00100000) {
									static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00100000) {
										static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00100000) {
											static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00100000) {
												static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00100000) {
													static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00100000) {
														static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00100000) {
															static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[3] != nullptr) {
		localMinPoint = glm::vec3(minPoint.x, minPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, midPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00010000) {
				static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00010000) {
					static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00010000) {
						static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00010000) {
							static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00010000) {
								static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00010000) {
									static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00010000) {
										static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00010000) {
											static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00010000) {
												static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00010000) {
													static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00010000) {
														static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00010000) {
															static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[4] != nullptr) {
		localMinPoint = glm::vec3(minPoint.x, midPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, maxPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00001000) {
				static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00001000) {
					static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00001000) {
						static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00001000) {
							static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00001000) {
								static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00001000) {
									static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00001000) {
										static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00001000) {
											static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00001000) {
												static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00001000) {
													static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00001000) {
														static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00001000) {
															static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[5] != nullptr) {
		localMinPoint = glm::vec3(midPoint.x, midPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, maxPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00000100) {
				static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00000100) {
					static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00000100) {
						static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00000100) {
							static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00000100) {
								static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00000100) {
									static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00000100) {
										static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00000100) {
											static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00000100) {
												static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00000100) {
													static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00000100) {
														static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00000100) {
															static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[6] != nullptr) {
		localMinPoint = midPoint;
		localMaxPoint = maxPoint;

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00000010) {
				static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00000010) {
					static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00000010) {
						static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00000010) {
							static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00000010) {
								static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00000010) {
									static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00000010) {
										static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00000010) {
											static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00000010) {
												static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00000010) {
													static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00000010) {
														static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00000010) {
															static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[7] != nullptr) {
		localMinPoint = glm::vec3(minPoint.x, midPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, maxPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00000001) {
				static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00000001) {
					static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00000001) {
						static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00000001) {
							static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00000001) {
								static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00000001) {
									static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00000001) {
										static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00000001) {
											static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00000001) {
												static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00000001) {
													static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00000001) {
														static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00000001) {
															static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (rayCollisionNodes->size() >= 0) {
		int shortestIndex = -1;
		float currentDistance = 0.0f;
		for (int i = 0; i < rayCollisionNodes->size(); i++) {
			currentDistance = glm::length(glm::cross(*((static_cast<Node*>(rayCollisionNodes->at(i)))->position) - origin, rayDirection - origin)) / glm::length(rayDirection - origin); // scaling helps with accuracy! BUT SHOULDNT BE DONE HERE
			if (currentDistance < maxDistanceFromRay) {
				shortestIndex = i;
				maxDistanceFromRay = currentDistance; // updates maximum to make sure we only get the closest result in the range!
			}
		}
		if (shortestIndex == -1) {
			delete rayCollisionNodes;
			return nullptr; // maybe a better way of doing this, nah, just check if return value is nullptr which means no successful collisions!
		}
		else {
			void* returnData = (static_cast<Node*>(rayCollisionNodes->at(shortestIndex)))->dataPointer; // pointer to data that was closest and in range;
			delete rayCollisionNodes;
			return returnData;
		}
	}
	else {
		delete rayCollisionNodes;
		return nullptr;
	}
}

std::vector<void*> Octree::RayCastRange(glm::vec3 origin, glm::vec3 rayDirection, float maxDistanceFromRay)
{
	glm::vec3 ray = rayDirection - origin;

	glm::vec3 raysToPoints[8]{};

	glm::vec3 localMinPoint{};
	glm::vec3 localMaxPoint{};

	glm::vec3 result{};

	std::vector<Node*>* rayCollisionNodes = new std::vector<Node*>{};

	if (storedData[0] != nullptr) {
		localMinPoint = minPoint;
		localMaxPoint = midPoint;

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b10000000) {
				static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b10000000) {
					static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b10000000) {
						static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b10000000) {
							static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b10000000) {
								static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b10000000) {
									static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b10000000) {
										static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b10000000) {
											static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b10000000) {
												static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b10000000) {
													static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b10000000) {
														static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b10000000) {
															static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[0]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[1] != nullptr) {
		localMinPoint = glm::vec3(midPoint.x, minPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, midPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b01000000) {
				static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b01000000) {
					static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b01000000) {
						static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b01000000) {
							static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b01000000) {
								static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b01000000) {
									static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b01000000) {
										static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b01000000) {
											static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b01000000) {
												static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b01000000) {
													static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b01000000) {
														static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b01000000) {
															static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[1]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[2] != nullptr) {
		localMinPoint = glm::vec3(midPoint.x, minPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, midPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00100000) {
				static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00100000) {
					static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00100000) {
						static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00100000) {
							static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00100000) {
								static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00100000) {
									static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00100000) {
										static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00100000) {
											static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00100000) {
												static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00100000) {
													static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00100000) {
														static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00100000) {
															static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[2]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[3] != nullptr) {
		localMinPoint = glm::vec3(minPoint.x, minPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, midPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00010000) {
				static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00010000) {
					static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00010000) {
						static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00010000) {
							static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00010000) {
								static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00010000) {
									static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00010000) {
										static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00010000) {
											static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00010000) {
												static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00010000) {
													static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00010000) {
														static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00010000) {
															static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[3]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[4] != nullptr) {
		localMinPoint = glm::vec3(minPoint.x, midPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, maxPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00001000) {
				static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00001000) {
					static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00001000) {
						static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00001000) {
							static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00001000) {
								static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00001000) {
									static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00001000) {
										static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00001000) {
											static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00001000) {
												static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00001000) {
													static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00001000) {
														static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00001000) {
															static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[4]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[5] != nullptr) {
		localMinPoint = glm::vec3(midPoint.x, midPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, maxPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00000100) {
				static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00000100) {
					static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00000100) {
						static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00000100) {
							static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00000100) {
								static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00000100) {
									static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00000100) {
										static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00000100) {
											static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00000100) {
												static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00000100) {
													static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00000100) {
														static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00000100) {
															static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[5]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[6] != nullptr) {
		localMinPoint = midPoint;
		localMaxPoint = maxPoint;

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00000010) {
				static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00000010) {
					static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00000010) {
						static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00000010) {
							static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00000010) {
								static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00000010) {
									static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00000010) {
										static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00000010) {
											static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00000010) {
												static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00000010) {
													static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00000010) {
														static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00000010) {
															static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[6]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[7] != nullptr) {
		localMinPoint = glm::vec3(minPoint.x, midPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, maxPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00000001) {
				static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
			}
			else {
				rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00000001) {
					static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
				}
				else {
					rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00000001) {
						static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
					}
					else {
						rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00000001) {
							static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
						}
						else {
							rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00000001) {
								static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
							}
							else {
								rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00000001) {
									static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
								}
								else {
									rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00000001) {
										static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
									}
									else {
										rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00000001) {
											static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
										}
										else {
											rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00000001) {
												static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
											}
											else {
												rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00000001) {
													static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
												}
												else {
													rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00000001) {
														static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
													}
													else {
														rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00000001) {
															static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, rayCollisionNodes);
														}
														else {
															rayCollisionNodes->push_back(static_cast<Node*>(storedData[7]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	std::vector<void*> returnVals{};
	if (rayCollisionNodes->size() >= 0) {
		float currentDistance = 0.0f;
		for (int i = 0; i < rayCollisionNodes->size(); i++) {
			currentDistance = glm::length(glm::cross(*((static_cast<Node*>(rayCollisionNodes->at(i)))->position) - origin, rayDirection - origin)) / glm::length(rayDirection - origin); // scaling helps with accuracy! BUT SHOULDNT BE DONE HERE
			if (currentDistance < maxDistanceFromRay) {
				returnVals.push_back((static_cast<Node*>(rayCollisionNodes->at(i)))->dataPointer);
			}
		}
	}
	delete rayCollisionNodes;
	return returnVals;
}


// PRIVATE

void Octree::FetchRayCastNodes(glm::vec3 origin, glm::vec3 rayDirection, std::vector<Node*>* nodesVector)
{
	glm::vec3 ray = rayDirection - origin;
	glm::vec3 raysToPoints[8]{};
	glm::vec3 localMinPoint{};
	glm::vec3 localMaxPoint{};
	glm::vec3 result{};

	if (storedData[0] != nullptr) {
		localMinPoint = minPoint;
		localMaxPoint = midPoint;

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b10000000) {
				static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
			}
			else {
				nodesVector->push_back(static_cast<Node*>(storedData[0]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b10000000) {
					static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
				}
				else {
					nodesVector->push_back(static_cast<Node*>(storedData[0]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b10000000) {
						static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
					}
					else {
						nodesVector->push_back(static_cast<Node*>(storedData[0]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b10000000) {
							static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
						}
						else {
							nodesVector->push_back(static_cast<Node*>(storedData[0]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b10000000) {
								static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
							}
							else {
								nodesVector->push_back(static_cast<Node*>(storedData[0]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b10000000) {
									static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
								}
								else {
									nodesVector->push_back(static_cast<Node*>(storedData[0]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b10000000) {
										static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
									}
									else {
										nodesVector->push_back(static_cast<Node*>(storedData[0]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b10000000) {
											static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
										}
										else {
											nodesVector->push_back(static_cast<Node*>(storedData[0]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b10000000) {
												static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
											}
											else {
												nodesVector->push_back(static_cast<Node*>(storedData[0]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b10000000) {
													static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
												}
												else {
													nodesVector->push_back(static_cast<Node*>(storedData[0]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b10000000) {
														static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
													}
													else {
														nodesVector->push_back(static_cast<Node*>(storedData[0]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b10000000) {
															static_cast<Octree*>(storedData[0])->FetchRayCastNodes(origin, rayDirection, nodesVector);
														}
														else {
															nodesVector->push_back(static_cast<Node*>(storedData[0]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[1] != nullptr) {
		localMinPoint = glm::vec3(midPoint.x, minPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, midPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b01000000) {
				static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
			}
			else {
				nodesVector->push_back(static_cast<Node*>(storedData[1]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b01000000) {
					static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
				}
				else {
					nodesVector->push_back(static_cast<Node*>(storedData[1]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b01000000) {
						static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
					}
					else {
						nodesVector->push_back(static_cast<Node*>(storedData[1]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b01000000) {
							static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
						}
						else {
							nodesVector->push_back(static_cast<Node*>(storedData[1]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b01000000) {
								static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
							}
							else {
								nodesVector->push_back(static_cast<Node*>(storedData[1]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b01000000) {
									static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
								}
								else {
									nodesVector->push_back(static_cast<Node*>(storedData[1]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b01000000) {
										static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
									}
									else {
										nodesVector->push_back(static_cast<Node*>(storedData[1]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b01000000) {
											static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
										}
										else {
											nodesVector->push_back(static_cast<Node*>(storedData[1]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b01000000) {
												static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
											}
											else {
												nodesVector->push_back(static_cast<Node*>(storedData[1]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b01000000) {
													static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
												}
												else {
													nodesVector->push_back(static_cast<Node*>(storedData[1]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b01000000) {
														static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
													}
													else {
														nodesVector->push_back(static_cast<Node*>(storedData[1]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b01000000) {
															static_cast<Octree*>(storedData[1])->FetchRayCastNodes(origin, rayDirection, nodesVector);
														}
														else {
															nodesVector->push_back(static_cast<Node*>(storedData[1]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[2] != nullptr) {
		localMinPoint = glm::vec3(midPoint.x, minPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, midPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00100000) {
				static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
			}
			else {
				nodesVector->push_back(static_cast<Node*>(storedData[2]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00100000) {
					static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
				}
				else {
					nodesVector->push_back(static_cast<Node*>(storedData[2]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00100000) {
						static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
					}
					else {
						nodesVector->push_back(static_cast<Node*>(storedData[2]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00100000) {
							static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
						}
						else {
							nodesVector->push_back(static_cast<Node*>(storedData[2]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00100000) {
								static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
							}
							else {
								nodesVector->push_back(static_cast<Node*>(storedData[2]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00100000) {
									static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
								}
								else {
									nodesVector->push_back(static_cast<Node*>(storedData[2]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00100000) {
										static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
									}
									else {
										nodesVector->push_back(static_cast<Node*>(storedData[2]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00100000) {
											static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
										}
										else {
											nodesVector->push_back(static_cast<Node*>(storedData[2]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00100000) {
												static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
											}
											else {
												nodesVector->push_back(static_cast<Node*>(storedData[2]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00100000) {
													static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
												}
												else {
													nodesVector->push_back(static_cast<Node*>(storedData[2]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00100000) {
														static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
													}
													else {
														nodesVector->push_back(static_cast<Node*>(storedData[2]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00100000) {
															static_cast<Octree*>(storedData[2])->FetchRayCastNodes(origin, rayDirection, nodesVector);
														}
														else {
															nodesVector->push_back(static_cast<Node*>(storedData[2]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[3] != nullptr) {
		localMinPoint = glm::vec3(minPoint.x, minPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, midPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00010000) {
				static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
			}
			else {
				nodesVector->push_back(static_cast<Node*>(storedData[3]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00010000) {
					static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
				}
				else {
					nodesVector->push_back(static_cast<Node*>(storedData[3]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00010000) {
						static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
					}
					else {
						nodesVector->push_back(static_cast<Node*>(storedData[3]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00010000) {
							static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
						}
						else {
							nodesVector->push_back(static_cast<Node*>(storedData[3]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00010000) {
								static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
							}
							else {
								nodesVector->push_back(static_cast<Node*>(storedData[3]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00010000) {
									static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
								}
								else {
									nodesVector->push_back(static_cast<Node*>(storedData[3]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00010000) {
										static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
									}
									else {
										nodesVector->push_back(static_cast<Node*>(storedData[3]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00010000) {
											static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
										}
										else {
											nodesVector->push_back(static_cast<Node*>(storedData[3]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00010000) {
												static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
											}
											else {
												nodesVector->push_back(static_cast<Node*>(storedData[3]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00010000) {
													static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
												}
												else {
													nodesVector->push_back(static_cast<Node*>(storedData[3]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00010000) {
														static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
													}
													else {
														nodesVector->push_back(static_cast<Node*>(storedData[3]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00010000) {
															static_cast<Octree*>(storedData[3])->FetchRayCastNodes(origin, rayDirection, nodesVector);
														}
														else {
															nodesVector->push_back(static_cast<Node*>(storedData[3]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[4] != nullptr) {
		localMinPoint = glm::vec3(minPoint.x, midPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, maxPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00001000) {
				static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
			}
			else {
				nodesVector->push_back(static_cast<Node*>(storedData[4]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00001000) {
					static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
				}
				else {
					nodesVector->push_back(static_cast<Node*>(storedData[4]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00001000) {
						static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
					}
					else {
						nodesVector->push_back(static_cast<Node*>(storedData[4]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00001000) {
							static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
						}
						else {
							nodesVector->push_back(static_cast<Node*>(storedData[4]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00001000) {
								static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
							}
							else {
								nodesVector->push_back(static_cast<Node*>(storedData[4]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00001000) {
									static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
								}
								else {
									nodesVector->push_back(static_cast<Node*>(storedData[4]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00001000) {
										static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
									}
									else {
										nodesVector->push_back(static_cast<Node*>(storedData[4]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00001000) {
											static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
										}
										else {
											nodesVector->push_back(static_cast<Node*>(storedData[4]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00001000) {
												static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
											}
											else {
												nodesVector->push_back(static_cast<Node*>(storedData[4]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00001000) {
													static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
												}
												else {
													nodesVector->push_back(static_cast<Node*>(storedData[4]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00001000) {
														static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
													}
													else {
														nodesVector->push_back(static_cast<Node*>(storedData[4]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00001000) {
															static_cast<Octree*>(storedData[4])->FetchRayCastNodes(origin, rayDirection, nodesVector);
														}
														else {
															nodesVector->push_back(static_cast<Node*>(storedData[4]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[5] != nullptr) {
		localMinPoint = glm::vec3(midPoint.x, midPoint.y, minPoint.z);
		localMaxPoint = glm::vec3(maxPoint.x, maxPoint.y, midPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00000100) {
				static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
			}
			else {
				nodesVector->push_back(static_cast<Node*>(storedData[5]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00000100) {
					static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
				}
				else {
					nodesVector->push_back(static_cast<Node*>(storedData[5]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00000100) {
						static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
					}
					else {
						nodesVector->push_back(static_cast<Node*>(storedData[5]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00000100) {
							static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
						}
						else {
							nodesVector->push_back(static_cast<Node*>(storedData[5]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00000100) {
								static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
							}
							else {
								nodesVector->push_back(static_cast<Node*>(storedData[5]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00000100) {
									static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
								}
								else {
									nodesVector->push_back(static_cast<Node*>(storedData[5]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00000100) {
										static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
									}
									else {
										nodesVector->push_back(static_cast<Node*>(storedData[5]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00000100) {
											static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
										}
										else {
											nodesVector->push_back(static_cast<Node*>(storedData[5]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00000100) {
												static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
											}
											else {
												nodesVector->push_back(static_cast<Node*>(storedData[5]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00000100) {
													static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
												}
												else {
													nodesVector->push_back(static_cast<Node*>(storedData[5]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00000100) {
														static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
													}
													else {
														nodesVector->push_back(static_cast<Node*>(storedData[5]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00000100) {
															static_cast<Octree*>(storedData[5])->FetchRayCastNodes(origin, rayDirection, nodesVector);
														}
														else {
															nodesVector->push_back(static_cast<Node*>(storedData[5]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[6] != nullptr) {
		localMinPoint = midPoint;
		localMaxPoint = maxPoint;

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00000010) {
				static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
			}
			else {
				nodesVector->push_back(static_cast<Node*>(storedData[6]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00000010) {
					static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
				}
				else {
					nodesVector->push_back(static_cast<Node*>(storedData[6]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00000010) {
						static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
					}
					else {
						nodesVector->push_back(static_cast<Node*>(storedData[6]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00000010) {
							static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
						}
						else {
							nodesVector->push_back(static_cast<Node*>(storedData[6]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00000010) {
								static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
							}
							else {
								nodesVector->push_back(static_cast<Node*>(storedData[6]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00000010) {
									static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
								}
								else {
									nodesVector->push_back(static_cast<Node*>(storedData[6]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00000010) {
										static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
									}
									else {
										nodesVector->push_back(static_cast<Node*>(storedData[6]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00000010) {
											static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
										}
										else {
											nodesVector->push_back(static_cast<Node*>(storedData[6]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00000010) {
												static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
											}
											else {
												nodesVector->push_back(static_cast<Node*>(storedData[6]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00000010) {
													static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
												}
												else {
													nodesVector->push_back(static_cast<Node*>(storedData[6]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00000010) {
														static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
													}
													else {
														nodesVector->push_back(static_cast<Node*>(storedData[6]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00000010) {
															static_cast<Octree*>(storedData[6])->FetchRayCastNodes(origin, rayDirection, nodesVector);
														}
														else {
															nodesVector->push_back(static_cast<Node*>(storedData[6]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (storedData[7] != nullptr) {
		localMinPoint = glm::vec3(minPoint.x, midPoint.y, midPoint.z);
		localMaxPoint = glm::vec3(midPoint.x, maxPoint.y, maxPoint.z);

		raysToPoints[0] = localMinPoint - origin;
		raysToPoints[1] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMinPoint.z) - origin;
		raysToPoints[2] = glm::vec3(localMaxPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
		result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[2])) * ray;
		if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
			if (branchState & 0b00000001) {
				static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
			}
			else {
				nodesVector->push_back(static_cast<Node*>(storedData[7]));
			}
		}
		else {
			raysToPoints[3] = glm::vec3(localMinPoint.x, localMinPoint.y, localMaxPoint.z) - origin;
			result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[2], raysToPoints[3])) * ray;
			if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
				if (branchState & 0b00000001) {
					static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
				}
				else {
					nodesVector->push_back(static_cast<Node*>(storedData[7]));
				}
			}
			else {
				raysToPoints[6] = localMaxPoint - origin;
				result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[2], raysToPoints[6])) * ray;
				if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
					if (branchState & 0b00000001) {
						static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
					}
					else {
						nodesVector->push_back(static_cast<Node*>(storedData[7]));
					}
				}
				else {
					raysToPoints[5] = glm::vec3(localMaxPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
					result = glm::inverse(glm::mat3x3(raysToPoints[1], raysToPoints[6], raysToPoints[5])) * ray;
					if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
						if (branchState & 0b00000001) {
							static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
						}
						else {
							nodesVector->push_back(static_cast<Node*>(storedData[7]));
						}
					}
					else {
						result = glm::inverse(glm::mat3x3(raysToPoints[2], raysToPoints[3], raysToPoints[6])) * ray;
						if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
							if (branchState & 0b00000001) {
								static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
							}
							else {
								nodesVector->push_back(static_cast<Node*>(storedData[7]));
							}
						}
						else {
							raysToPoints[7] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMaxPoint.z) - origin;
							result = glm::inverse(glm::mat3x3(raysToPoints[3], raysToPoints[6], raysToPoints[7])) * ray;
							if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
								if (branchState & 0b00000001) {
									static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
								}
								else {
									nodesVector->push_back(static_cast<Node*>(storedData[7]));
								}
							}
							else {
								result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[1], raysToPoints[5])) * ray;
								if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
									if (branchState & 0b00000001) {
										static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
									}
									else {
										nodesVector->push_back(static_cast<Node*>(storedData[7]));
									}
								}
								else {
									raysToPoints[4] = glm::vec3(localMinPoint.x, localMaxPoint.y, localMinPoint.z) - origin;
									result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[5], raysToPoints[4])) * ray;
									if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
										if (branchState & 0b00000001) {
											static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
										}
										else {
											nodesVector->push_back(static_cast<Node*>(storedData[7]));
										}
									}
									else {
										result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[3], raysToPoints[7])) * ray;
										if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
											if (branchState & 0b00000001) {
												static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
											}
											else {
												nodesVector->push_back(static_cast<Node*>(storedData[7]));
											}
										}
										else {
											result = glm::inverse(glm::mat3x3(raysToPoints[0], raysToPoints[7], raysToPoints[4])) * ray;
											if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
												if (branchState & 0b00000001) {
													static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
												}
												else {
													nodesVector->push_back(static_cast<Node*>(storedData[7]));
												}
											}
											else {
												result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[5], raysToPoints[6])) * ray;
												if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
													if (branchState & 0b00000001) {
														static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
													}
													else {
														nodesVector->push_back(static_cast<Node*>(storedData[7]));
													}
												}
												else {
													result = glm::inverse(glm::mat3x3(raysToPoints[4], raysToPoints[6], raysToPoints[7])) * ray;
													if (result.x >= 0.0f && result.y >= 0.0f && result.z >= 0.0f) {
														if (branchState & 0b00000001) {
															static_cast<Octree*>(storedData[7])->FetchRayCastNodes(origin, rayDirection, nodesVector);
														}
														else {
															nodesVector->push_back(static_cast<Node*>(storedData[7]));
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}