#include <glad/glad.h>
#include <KHR/khrplatform.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

#include "GLFWLoader.h"
#include "Window.h"
#include "InputCallback/InputCallback.h"
#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"
#include "Model.h"

extern Camera m_Camera;
#define LOG(x) std::cout << (x) << std::endl

GLFWLoader* GLFWLoader::m_Loader = nullptr;
unsigned int height = 600;
unsigned int width = 800;

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); ++i)
	{
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

float vertices[] = {
	//  --------坐标--------    --纹理坐标--     
		 // Back face
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
static unsigned int indices[] = {
	0, 1, 2,
	3, 4, 5,
	6, 7, 8,
	9, 10, 11,
	12, 13, 14,
	15, 16, 17,
	18, 19, 20,
	21, 22, 23,
	24, 25, 26,
	27, 28, 29,
	30, 31, 32,
	33, 34, 35
};
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

std::vector<std::string> faces
{
	"Source/SkyBox/skybox/right.jpg",
	"Source/SkyBox/skybox/left.jpg",
	"Source/SkyBox/skybox/top.jpg",
	"Source/SkyBox/skybox/bottom.jpg",
	// warning: distinguish with tutorial
	"Source/SkyBox/skybox/front.jpg",
	"Source/SkyBox/skybox/back.jpg"
};

int main()
{
	GLFWLoader* glfwLoader = GLFWLoader::createGLFWLoader(800, 600, "OpenGLWindow");
	Window& window = glfwLoader->getWindow();

	window.setView(800, 600);
	window.setColor(0.6, 0.6, 0.3, 1.0f);
	window.setWindowResizeCallback(framebuffer_size_callback);
	window.setMouseCallback();
	window.setScrollCallback();

	ShaderProgram shaderProgram("Shader/Skybox/vertexShader.vert", "Shader/Skybox/fragmentShader.frag");
	ShaderProgram boxShader("Shader/Skybox/normalVertexShader.vert", "Shader/Skybox/normalFragmentShader.frag");
	Model nanosuit("Source/model/nanosuit_reflection/nanosuit.obj");


	VertexArray boxvao;
	VertexBuffer boxvbo(vertices, sizeof(vertices));
	VertexBufferLayout boxlayout;
	boxlayout.Push<float>(3);
	boxlayout.Push<float>(3);
	boxvao.BindBuffer(boxvbo, boxlayout);
	IndexBuffer boxibo(indices, sizeof(indices) / sizeof(unsigned int));


	// 创建一个立方体贴图
	unsigned int cubemapTexture = loadCubemap(faces);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window.getWindowPtr()))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		window.ProcessInput();

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera.fov), (float)width / height, 0.01f, 100.0f);
		glm::mat4 view = window.GenerateViewMat();
		glEnable(GL_DEPTH_TEST);
		glm::mat4 model(1.0f);
		view = window.GenerateViewMat();
		boxShader.Use();
		boxShader.SetInt("skybox", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		boxShader.SetUniform4m("model", model);
		boxShader.SetUniform4m("view", view);
		boxShader.SetUniform4m("projection", projection);
		boxShader.SetUniform3v("cameraPos", m_Camera.Position);
		nanosuit.Draw(boxShader);
		//Renderer::render(boxvao, boxibo, boxShader);

		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		shaderProgram.Use();
		// 设置观察和投影矩阵
		view = window.GenerateViewMat();
		view = glm::mat4(glm::mat3(view));
		shaderProgram.SetUniform4m("view", view);
		shaderProgram.SetUniform4m("projection", projection);
		glBindVertexArray(vao);
		shaderProgram.SetInt("skybox", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
		glfwPollEvents();
		glfwSwapBuffers(window.getWindowPtr());
	}

	GLFWLoader::deleteGLFWLoader();

	return 0;
}

//float vertices[] = {
//	//  --------坐标--------    --纹理坐标--     
//		 // Back face
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//	// Front face
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
//	// Left face
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
//	// Right face
//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
//	 // Bottom face
//	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
//	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
//	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
//	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
//	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
//	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
//	 // Top face
//	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
//	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//	 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left      
//};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
//
//static unsigned int indices[] = {
//	0, 1, 2,
//	3, 4, 5,
//	6, 7, 8,
//	9, 10, 11,
//	12, 13, 14,
//	15, 16, 17,
//	18, 19, 20,
//	21, 22, 23,
//	24, 25, 26,
//	27, 28, 29,
//	30, 31, 32,
//	33, 34, 35
//};

float floorVertices[] =
{
	5.0f, -0.6f, 5.0f,
	5.0f, -0.6f, -5.0f,
	-5.0f, -0.6f, -5.0f,
	-5.0f, -0.6f, 5.0f
};

unsigned int floorIndices[] =
{
	0, 1, 2,
	2, 3, 0
};

float TransVertices[] =
{
	-0.5, -0.5, 0.0,		0.0, 0.0,
	0.5, -0.5, 0.0,			1.0, 0.0,
	0.5, 0.5, 0.0,			1.0, 1.0,
	-0.5, 0.5, 0.0,			0.0, 1.0
};

unsigned int TransIndices[] = {
	0, 1, 2,
	2, 3, 0
};



glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

float testTri[] =
{
	-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
};

unsigned int testIndex[] = {
	0, 1, 2
};


float screenVertices[] =
{
	-1.0f, -1.0f,	0.0, 0.0,
	1.0f, -1.0f,	1.0, 0.0,
	1.0f, 1.0f,		1.0, 1.0,
	-1.0f, 1.0f,	0.0, 1.0
};

unsigned int screenIndices[] =
{
	0, 1, 2,
	2, 3, 0
};
int frameBuffer1()
{
	GLFWLoader* glfwLoader = GLFWLoader::createGLFWLoader(800, 600, "OpenGLWindow");
	Window& window = glfwLoader->getWindow();

	window.setView(800, 600);
	window.setColor(0.6, 0.6, 0.3, 1.0f);
	window.setWindowResizeCallback(framebuffer_size_callback);
	window.setMouseCallback();
	window.setScrollCallback();

	ShaderProgram shaderProgram("Shader/FrameBuffer/CubeVertexShader.vert", "Shader/FrameBuffer/fragmentShader.frag");
	ShaderProgram screenShader("Shader/FrameBuffer/vertexShader.vert", "Shader/FrameBuffer/ScreenFragmentShader.frag");
	Texture texture("Source/image/1.jpg");

	VertexBuffer scrVbo(screenVertices, sizeof(screenVertices));
	IndexBuffer srcIbo(screenIndices, sizeof(screenIndices) / sizeof(unsigned int));
	VertexBufferLayout srcLayout;
	srcLayout.Push<float>(2);
	srcLayout.Push<float>(2);
	VertexArray srcVao;
	srcVao.BindBuffer(scrVbo, srcLayout);

	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices) / sizeof(unsigned int));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	VertexArray vao;
	vao.BindBuffer(vbo, layout);

	// 创建一个framebuffer对象
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// 生成纹理
	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 将他附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// 创建一个渲染缓冲对象
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// 将渲染缓冲对象附加到帧缓冲的深度和模板附件上
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// 检测帧缓冲是否完整（有所有测试）
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window.getWindowPtr()))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		window.ProcessInput();
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glm::mat4 model(1.0f);
		glm::mat4 view = window.GenerateViewMat();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(m_Camera.fov), (float)width / height, 0.1f, 100.0f);
		model = glm::translate(model, { 0.0f, 0.0f, 0.0f });
		//model = glm::rotate(model, glm::radians(45.0f), {0.0f, 1.0f, 0.0f});
		model = glm::scale(model, { (float)height / width, 1, 100.0f / width });
		shaderProgram.SetUniform4m("model", model);
		//shaderProgram.SetUniform4m("projection", projection);
		shaderProgram.SetInt("screenTexture", 0);
		texture.Bind(0);
		Renderer::render(vao, ibo, shaderProgram);

		// 返回默认
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		Renderer::render(srcVao, srcIbo, screenShader);

		glfwPollEvents();
		glfwSwapBuffers(window.getWindowPtr());
	}

	GLFWLoader::deleteGLFWLoader();

	return 0;
}


int old()
{
	GLFWLoader* glfwLoader = GLFWLoader::createGLFWLoader(800, 600, "OpenGLWindow");

	Window window = glfwLoader->getWindow();
	window.setView(800, 600);
	window.setColor(1.0, 1.0, 1.0, 1.0);
	window.setWindowResizeCallback(framebuffer_size_callback);
	window.setMouseCallback();
	window.setScrollCallback();
	// set 
	// --- shader
	// --- texture
	// --- vertex array object
	// --- vertex buffer object
	// --- index buffer object
	// --- layout object
	ShaderProgram shaderProgram("Shader/VertexShader/LightCombVertShader.vert", "Shader/FragmentShader/LightCombFragShader.frag");
	Texture texture("Source/image/container.png");
	Texture specularTexture("Source/image/container_specular.png");
	Texture AnimeTexture("Source/image/1.jpg");

	VertexArray vao;
	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices) / sizeof(unsigned int));
	VertexBufferLayout Layout;
	Layout.Push<float>(3);
	Layout.Push<float>(3);
	Layout.Push<float>(2);
	Layout.Push<float>(3);
	vao.BindBuffer(vbo, Layout);


	ShaderProgram lightShader("Shader/LightShader/VertexShader.vert", "Shader/LightShader/FragmentShader.frag");
	VertexArray light_vao;
	light_vao.BindBuffer(vbo, Layout);

	// DirectLight
	shaderProgram.SetUniform3v("dirLight.ambient", { 0.1f, 0.1f, 0.1f });
	shaderProgram.SetUniform3v("dirLight.diffuse", { 0.5f, 0.5f, 0.5f });
	shaderProgram.SetUniform3v("dirLight.specular", { 1.0f, 1.0f, 1.0f });
	shaderProgram.SetUniform3v("dirLight.direction", { 0.0f, 0.0f, -1.0f });

	// PointLight
	for (int i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); ++i)
	{
		std::string lightName = "pointLights[" + std::to_string(i) + "].";
		shaderProgram.SetUniform3v(lightName + "ambient", { 0.1f, 0.1f, 0.1f });
		shaderProgram.SetUniform3v(lightName + "diffuse", { 0.2f, 0.2f, 0.2f });
		shaderProgram.SetUniform3v(lightName + "specular", { 0.8f, 0.8f, 0.8f });
		shaderProgram.SetUniform3v(lightName + "position", pointLightPositions[i]);
		shaderProgram.SetFloat(lightName + "constant", 1.0f);
		shaderProgram.SetFloat(lightName + "linear", 0.14f);
		shaderProgram.SetFloat(lightName + "quadratic", 0.07f);
	}

	// SpotLight
	shaderProgram.SetUniform3v("spotLight.ambient", { 0.2f, 0.0f, 0.2f });
	shaderProgram.SetUniform3v("spotLight.diffuse", { 0.5f, 0.0f, 0.2f });
	shaderProgram.SetUniform3v("spotLight.specular", { 0.5f, 0.0f, 0.2f });
	shaderProgram.SetUniform3v("spotLight.position", m_Camera.Position);
	shaderProgram.SetUniform3v("spotLight.direction", m_Camera.Front);
	shaderProgram.SetFloat("spotLight.cutOff", glm::cos(glm::radians(7.5f)));
	shaderProgram.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	shaderProgram.SetInt("material.specular", 0);
	shaderProgram.SetInt("material.diffuse", 1);
	shaderProgram.SetInt("AnimeTexture", 2);

	shaderProgram.SetFloat("material.shininess", 64.0f);

	// load models;
	Model ourModel("Source/model/nanosuit/nanosuit.obj");

	glEnable(GL_DEPTH_TEST);
	double timeInNano = 0;
	double timeElapse = 0;
	while (!glfwWindowShouldClose(window.getWindowPtr()))
	{
		window.ProcessInput();

		specularTexture.Bind(0);
		texture.Bind(1);
		AnimeTexture.Bind(2);

		glm::mat4 view = window.GenerateViewMat();

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(m_Camera.fov), (float)width / height, 0.1f, 100.0f);
		shaderProgram.SetUniform3v("viewPos", m_Camera.Position);
		shaderProgram.SetUniform3v("spotLight.position", m_Camera.Position);
		shaderProgram.SetUniform3v("spotLight.direction", m_Camera.Front);

		for (unsigned int i = 0; i < 10; ++i)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.5f, 0.3f));
			shaderProgram.SetUniform4m("MVP", projection * view * model);
			shaderProgram.SetUniform4m("model", model);

			Renderer::render(vao, ibo, shaderProgram);
		}
		for (unsigned int i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); ++i)
		{
			glm::mat4 lightModel(1);
			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			lightShader.SetUniform4m("MVP", projection * view * lightModel);
			Renderer::render(light_vao, ibo, lightShader);
		}
		glm::mat4 model(1.0f);
		shaderProgram.SetUniform4m("MVP", projection * view * model);
		ourModel.Draw(shaderProgram);

		glfwPollEvents();
		glfwSwapBuffers(window.getWindowPtr());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	GLFWLoader::deleteGLFWLoader();

	return 0;
}


















//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//   int nrAttributes;
//   glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//   std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
//// ans: 16
