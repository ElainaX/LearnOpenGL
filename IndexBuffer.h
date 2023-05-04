#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IndexBuffer
{
public:
	IndexBuffer() { m_IBO = m_Count = 0; }
	IndexBuffer(const void* indexData, unsigned int count);
	~IndexBuffer();
	void Assign(const void* indexData, unsigned int count);
	void Bind() const;
	void UnBind() const;
	unsigned int getCount() const { return m_Count; }
private:
	unsigned int m_IBO;
	unsigned int m_Count;
};