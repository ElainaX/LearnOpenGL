#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <cmath>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Camera.h"


void mouse_callback(GLFWwindow* window, double xpos, double ypos);

class Window
{
	friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);

public:

	Window(GLFWwindow* window = nullptr)
		: m_Window(window) {
	}

	Window(const Window& w)
		: m_Window(w.m_Window)
	{

	}

	GLFWwindow* getWindowPtr()
	{
		return m_Window;
	}

	void setColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void setView(unsigned int width = 800, unsigned int height = 600)
	{
		glViewport(0, 0, width, height);
	}

	void setWindowResizeCallback(void (*resizeFunc)(GLFWwindow*, int, int))
	{
		glfwSetFramebufferSizeCallback(m_Window, resizeFunc);
	}

	void setMouseCallback();

	void setScrollCallback();

public:
	void ProcessInput();
	void Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	glm::mat4 GenerateViewMat();

private:


private:
	GLFWwindow* m_Window;

};