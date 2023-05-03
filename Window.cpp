#include "Window.h"
#include <functional>
Camera m_Camera;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static float deltaTime = 0;
	static double elapsedTime = 0;
	deltaTime = glfwGetTime() - elapsedTime;
	elapsedTime = glfwGetTime();

	static float lastX = xpos, lastY = ypos;
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xOffset *= m_Camera.Sensitivity;
	yOffset *= m_Camera.Sensitivity;
	

	m_Camera.processMouseMovement(xOffset, yOffset);
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (m_Camera.fov >= 1.0f && m_Camera.fov <= 45.0f)
		m_Camera.fov -= yoffset;
	if (m_Camera.fov <= 1.0f)
		m_Camera.fov = 1.0f;
	if (m_Camera.fov >= 45.0f)
		m_Camera.fov = 45.0f;

}

void Window::setMouseCallback()
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_Window, mouse_callback);
}


void Window::setScrollCallback()
{
	glfwSetScrollCallback(m_Window, scroll_callback);
}

void Window::ProcessInput()
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, true);
	static float deltaTime = 0;
	static double elapsedTime = 0;
	deltaTime = glfwGetTime() - elapsedTime;
	elapsedTime = glfwGetTime();
	m_Camera.Speed = 3.0f * deltaTime;

	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		m_Camera.Position += m_Camera.Speed * m_Camera.Front;
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		m_Camera.Position -= m_Camera.Speed * m_Camera.Front;
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		m_Camera.Position -= glm::normalize(glm::cross(m_Camera.Front, m_Camera.Up)) * m_Camera.Speed;
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		m_Camera.Position += glm::normalize(glm::cross(m_Camera.Front, m_Camera.Up)) * m_Camera.Speed;
	if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
		m_Camera.Position += m_Camera.Up * m_Camera.Speed;
	if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS)
		m_Camera.Position -= m_Camera.Up * m_Camera.Speed;
}

glm::mat4 Window::GenerateViewMat()
{
	return glm::lookAt(m_Camera.Position, m_Camera.Position + m_Camera.Front, m_Camera.Up);
}

