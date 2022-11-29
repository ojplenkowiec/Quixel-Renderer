#ifndef MESH_H
#define MESH_H

#include <vector>
#include "glm.hpp"

class Mesh3D { // read in from file!
private:
	glm::vec3 m_VertexPositions;
	glm::vec3 m_IndexData;
	glm::vec3 m_NormalData;
public:
	Mesh3D();
	~Mesh3D();

	void LoadFromObj();
};

#endif