#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "debugging.h"

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void SubData(const void* data, unsigned int size);

	void Bind() const;
	void Unbind() const;
};

#endif