#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "debugging.h"

class VertexBuffer
{
private:
	unsigned int m_VertexCount;
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int vertexCount);
	~VertexBuffer();

	void SubData(const void* data, unsigned int size);

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetVertexCount() const { return m_VertexCount; }
};

#endif