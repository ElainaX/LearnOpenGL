#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	void Use() const { glUseProgram(m_ShaderProgram); }

public:
	void SetUniform4v(const std::string& uniformName, const glm::vec4& color) const;

private:
	unsigned int CompileShader(const char* shaderSource, int shaderType);
	unsigned int CreateProgram(const std::string& vertexShaderSource,  const std::string& fragmentShaderSource);
private:
	unsigned int m_ShaderProgram;
};
