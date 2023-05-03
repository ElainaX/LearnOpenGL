#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string& vertexShaderSourceFile, const std::string& fragmentShaderSourceFile)
{
	std::ifstream vertexShaderSourceCode(vertexShaderSourceFile);
	std::ifstream fragmentShaderSourceCode(fragmentShaderSourceFile);

	std::string vertexShaderSource((std::istreambuf_iterator<char>(vertexShaderSourceCode)),
		std::istreambuf_iterator<char>());
	std::string fragmentShaderSource((std::istreambuf_iterator<char>(fragmentShaderSourceCode)),
		std::istreambuf_iterator<char>());

	m_ShaderProgram = CreateProgram(vertexShaderSource, fragmentShaderSource);
}

void ShaderProgram::SetUniform4v(const std::string& uniformName, const glm::vec4& color) const
{
	Use();
	glUniform4f(glGetUniformLocation(m_ShaderProgram, uniformName.c_str()),
		color.r, color.g, color.b, color.t);
}

void ShaderProgram::SetUniform4m(const std::string& uniformName, const glm::mat4& mat) const
{
	Use();
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, uniformName.c_str()), 1, 
		GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::SetUniform3v(const std::string& uniformName, const glm::vec3& color) const
{
	Use();
	glUniform3f(glGetUniformLocation(m_ShaderProgram, uniformName.c_str()),
		color.r, color.g, color.b);
}

void ShaderProgram::SetFloat(const std::string& uniformName, float f)
{
	Use();
	glUniform1f(glGetUniformLocation(m_ShaderProgram, uniformName.c_str()), f);
}

void ShaderProgram::SetInt(const std::string& uniformName, int i)
{
	Use();
	glUniform1i(glGetUniformLocation(m_ShaderProgram, uniformName.c_str()), i);
}

unsigned int ShaderProgram::CompileShader(const char* shaderSource, int shaderType)
{
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	{
		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "shader Compile failed!" << std::endl;
		}
	}
	return shader;
}

unsigned int ShaderProgram::CreateProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	unsigned int vertexShader = CompileShader(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShader(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	{
		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char InfoLog[512];
			glGetProgramInfoLog(program, 512, NULL, InfoLog);
			std::cout << "program link failed: " << InfoLog << std::endl;
		}
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

