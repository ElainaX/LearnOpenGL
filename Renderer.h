#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

class Renderer
{
public:
	static void render(const VertexArray& vao, const IndexBuffer& ibo, const ShaderProgram& shader)
	{
		vao.Bind();
		ibo.Bind();
		shader.Use();

		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
	}
};