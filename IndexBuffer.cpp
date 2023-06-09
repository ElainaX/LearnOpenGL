#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* indexData, unsigned int count) : m_Count(count)
{
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indexData, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	if(m_IBO != 0) glDeleteBuffers(1, &m_IBO);
}

void IndexBuffer::Assign(const void* indexData, unsigned int count)
{
	if (m_IBO != 0) glDeleteBuffers(1, &m_IBO);
	m_Count = count;
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indexData, GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

