#include "vertexarray.h"

VertexArray::VertexArray()
	:m_VertexCount(0), m_InstanceCount(0), m_VertexAttribArrayCount(0)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout) 
	:m_VertexCount(0), m_InstanceCount(0), m_VertexAttribArrayCount(0)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	AddVertexBuffer(vb, layout);
}

VertexArray::VertexArray(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexLayout, const VertexBuffer& instanceVertexBuffer, const VertexBufferLayout& instanceVertexLayout)
	:m_VertexCount(0), m_InstanceCount(0), m_VertexAttribArrayCount(0)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	AddVertexBuffer(vertexBuffer, vertexLayout);
	AddInstanceVertexBuffer(instanceVertexBuffer, instanceVertexLayout);
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexLayout)
{
	m_VertexCount = vertexBuffer.GetVertexCount();
	Bind();
	vertexBuffer.Bind();
	const auto& elements = vertexLayout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		unsigned int currentVAA = i + m_VertexAttribArrayCount;
		const auto& element = elements[currentVAA];
		GLCall(glEnableVertexAttribArray(currentVAA));
		GLCall(glVertexAttribPointer(currentVAA, element.count, element.type, element.normalized, vertexLayout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	vertexBuffer.Unbind();
	Unbind();
	m_VertexAttribArrayCount += elements.size();
}

void VertexArray::AddInstanceVertexBuffer(const VertexBuffer& instanceVertexBuffer, const VertexBufferLayout& instanceVertexLayout)
{
	m_InstanceCount = instanceVertexBuffer.GetVertexCount();
	Bind();
	instanceVertexBuffer.Bind();
	const auto& elements = instanceVertexLayout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		unsigned int currentVAA = i + m_VertexAttribArrayCount;

		const auto& element = elements[i];

		GLCall(glEnableVertexAttribArray(currentVAA));
		GLCall(glVertexAttribPointer(currentVAA, element.count, element.type, element.normalized, instanceVertexLayout.GetStride(), (const void*)offset));

		glVertexAttribDivisor(currentVAA, 1); // tells VAO to only increment once per instance!

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	instanceVertexBuffer.Unbind();
	Unbind();
	m_VertexAttribArrayCount += elements.size();
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
