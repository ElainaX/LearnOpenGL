#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

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
	void SetUniform4m(const std::string& uniformName, const glm::mat4& mat) const;
	void SetUniform3v(const std::string& uniformName, const glm::vec3& color) const;
	void SetFloat(const std::string& uniformName, float f);
	void SetInt(const std::string& uniformName, int i);

private:
	unsigned int CompileShader(const char* shaderSource, int shaderType);
	unsigned int CreateProgram(const std::string& vertexShaderSource,  const std::string& fragmentShaderSource);
private:
	unsigned int m_ShaderProgram;
};
