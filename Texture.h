#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <iostream>
#include <string>
#include <cassert>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LoadBuffer;
	int m_Height, m_Width, m_NrChannels;

public:
	Texture()
		: m_RendererID(0), m_FilePath(""), m_LoadBuffer(nullptr), 
		m_Height(0), m_Width(0), m_NrChannels(0) {}
	Texture(const std::string& path);
	Texture(const Texture& t);
	~Texture();
	void Assign(const std::string& path);
	void Bind(unsigned int slot = 0);
	void UnBind();

	unsigned int GetHeight() const { return m_Height; }
};