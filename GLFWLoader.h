#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <string>
#include <iostream>

#include "Window.h"

class GLFWLoader
{
public:
	static GLFWLoader* createGLFWLoader(unsigned int width, unsigned int height, std::string&& name = "default Windows")
	{
		if (!m_Loader)
			m_Loader = new GLFWLoader(width, height, name);
		return m_Loader;
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

		assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
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