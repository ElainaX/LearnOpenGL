#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Renderer.h"

class Window
{
public:

	Window(GLFWwindow* window = nullptr)
		: m_Window(window) {}

	GLFWwindow* getWindowPtr()
	{
		return m_Window;
	}

	void setColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void setView(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void setWindowResizeCallback(void (*resizeFunc)(GLFWwindow*, int, int))
	{
		glfwSetFramebufferSizeCallback(m_Window, resizeFunc);
	}

	void Run(const VertexArray& vao, const IndexBuffer& ibo, ShaderProgram& shader)
	{

		while (!glfwWindowShouldClose(m_Window))
		{
			ProcessInput();
			
			Renderer::render(vao, ibo, shader);

			//Render();
			//glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwPollEvents(); // poll means ̽ѯ
			glfwSwapBuffers(m_Window);
		}
	}
private:
	void ProcessInput();
	void Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	GLFWwindow* m_Window;
};