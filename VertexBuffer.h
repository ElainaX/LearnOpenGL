#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VertexBuffer
{
public:
	VertexBuffer() { m_VBO = 0; }
	VertexBuffer(const void* data, int size);
	void Assign(const void* data, int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
private:
	unsigned int m_VBO;
};