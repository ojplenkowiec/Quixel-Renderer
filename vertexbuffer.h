#ifndef VERTEX_BUFFER
#define VERTEX_BUFFER

#include "debugging.h"

class VertexBuffer
{
private:
	unsigned int m_VertexCount;
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int vertexCount);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetVertexCount() const { return m_VertexCount; }
};

#endif