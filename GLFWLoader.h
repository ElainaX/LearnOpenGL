#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <string>
#include <iostream>

#include "Window.h"
#include "InputCallback/InputCallback.h"

class GLFWLoader
{
public:
	static GLFWLoader* createGLFWLoader(unsigned int width, unsigned int height, std::string&& name = "default Windows")
	{
		if (!m_Loader)
			m_Loader = new GLFWLoader(width, height, name);
		return m_Loader;
	}


	void setColor(float r, float g, float b, float a)
	{
		m_Window.setColor(r, g, b, a);
	}

	void setView(unsigned int width = 800, unsigned int height = 600)
	{
		m_Window.setView(width, height);
	}

	void setMouseCallback()
	{
		m_Window.setMouseCallback();
	}

	void setScrollCallback()
	{
		m_Window.setScrollCallback();
	}

	void setWindowResizeCallback(void (*resizeFunc)(GLFWwindow*, int, int))
	{
		m_Window.setWindowResizeCallback(resizeFunc);
	}

	static void deleteGLFWLoader()
	{
		delete m_Loader;
		m_Loader = nullptr;
	}



	GLFWwindow* getWindowPtr() { return m_Window.getWindowPtr(); }
	Window& getWindow() { return m_Window; }
	
private:
	void Init(unsigned int width, unsigned int height, std::string& name)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		if (window == nullptr) std::cerr << "Window initial failed!" << std::endl;
		glfwMakeContextCurrent(window);
		m_Window = Window(window);
		//assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "init glad successfully" << std::endl;
		}

	}

private:
	GLFWLoader(unsigned int width, unsigned int height, std::string& name) 
	{
		Init(width, height, name);
	}
	~GLFWLoader() {
		glfwTerminate();
	}
	static GLFWLoader* m_Loader;
	Window m_Window;
};