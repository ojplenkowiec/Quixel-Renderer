#ifndef VERTEX_ARRAY
#define VERTEX_ARRAY

#include "debugging.h"

#include "vertexbuffer.h"
#include "vertexbufferlayout.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout);
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};

#endif