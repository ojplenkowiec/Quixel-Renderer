#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <vector>
#include "glm.hpp"

/*
	Primitives are by default shapes of a unit size x = 0.0f - 1.0f, y = 0.0f - 1.0f
*/
class Primitive {
protected:
	std::vector<glm::vec3>* m_Vertices; // as vector also stores vertex count! useful!
	std::vector<
	Primitive() {
		m_Vertices = new std::vector<glm::vec3>();
	}
	~Primitive() {
		delete m_Vertices;
	}
public:
	inline uint32_t GetVertexCount() { return (m_Vertices->size()); } // used to get number of vertices, useful for gl calls
};

class Quad : public Primitive {

};

class Triangle : public Primitive {

};

class Circle : public Primitive {

};

class Ellipse : public Primitive {

};

class Polygon : public Primitive {

};

#endif

// Object = primitive + color/texture + rules + effects (outline... etc.)