#include "vertexbuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int vertexCount)
    :m_VertexCount(vertexCount)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // Binds a buffer object ID to the specified buffer binding point
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // Pushes data
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::SubData(const void* data, unsigned int size)
{
    GLCall(glNamedBufferSubData(m_RendererID, 0, size, data));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
