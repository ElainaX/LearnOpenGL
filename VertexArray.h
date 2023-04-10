#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void BindBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
	void Bind() const;
	void UnBind() const;

private:
	unsigned int m_VAO;
};