#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "debugging.h"

#include "vertexbuffer.h"
#include "vertexbufferlayout.h"

class VertexArray
{
private:
	unsigned int m_RendererID;

	unsigned int m_VertexCount;
	unsigned int m_InstanceCount;

	unsigned int m_VertexAttribArrayCount;
public:
	VertexArray();
	VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout);
	VertexArray(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexLayout, const VertexBuffer& instanceVertexBuffer, const VertexBufferLayout& instanceVertexLayout);
	~VertexArray();

	void AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddInstanceVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout); // unsure on name...?

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetVertexCount() const { return m_VertexCount; }
	inline unsigned int GetInstanceCount() const { return m_InstanceCount; }
};

#endif