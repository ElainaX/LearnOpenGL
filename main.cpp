#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include "GLFWLoader.h"
#include "Window.h"
#include "InputCallback/InputCallback.h"
#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

GLFWLoader* GLFWLoader::m_Loader = nullptr;

static float vertices[] = {
	-0.5f, -0.5f, 0.0f,			1.0f, 0.0f, 0.0f,			0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,			0.0f, 1.0f, 0.0f,			1.0f, 0.0f,
	0.5f, 0.5f, 0.0f,			0.0f, 0.0f, 1.0f,			1.0f, 1.0f,
	-0.5f, 0.5f, 0.0f,			1.0f, 1.0f, 0.0f,			0.0f, 1.0f
};

static unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};

void SetTextureMode()
{
	
}

int main()
{
	GLFWLoader* glfwLoader = GLFWLoader::createGLFWLoader(800, 600, "OpenGLWindow");
	Window window = glfwLoader->getWindow();

	// �ӿڣ��۲��õĴ��ڣ�
	window.setView(800, 600);
	window.setColor(0.3, 0.0, 0.1, 1.0);
	window.setWindowResizeCallback(framebuffer_size_callback);
	
	ShaderProgram shaderProgram("Shader/VertexShader/VertexShader1.vert", 
									"Shader/FragmentShader/FragmentShader1.frag");
	shaderProgram.Use();

	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices) / sizeof(unsigned int));

	VertexBufferLayout Layout;
	Layout.Push<float>(3);
	Layout.Push<float>(3);
	Layout.Push<float>(2);

	VertexArray vao;
	vao.BindBuffer(vbo, Layout);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//   int nrAttributes;
	//   glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//   std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	//// ans: 16

	window.Run(vao, ibo, shaderProgram);

	GLFWLoader::deleteGLFWLoader();

	return 0;
}

//// ʹ��VAO�������ݺ�����
//unsigned int VAO;
//glGenVertexArrays(1, &VAO);
//glBindVertexArray(VAO);
//unsigned int VBO;
//glGenBuffers(1, &VBO);
//glBindBuffer(GL_ARRAY_BUFFER, VBO);
//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//glEnableVertexAttribArray(0);
////glBindVertexArray(0);