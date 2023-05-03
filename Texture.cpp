#include "Texture.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LoadBuffer(nullptr),
	m_Width(0), m_Height(0), m_NrChannels(0)
{
	int GLMODE = GL_RGBA;
	if (path.find(".png") != std::string::npos)
		GLMODE = GL_RGBA;
	else if (path.find(".jpg") != std::string::npos)
		GLMODE = GL_RGB;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// load image
	stbi_set_flip_vertically_on_load(true);
	m_LoadBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);
	
	if (m_LoadBuffer != nullptr) {
		std::cout << "texture load successfully" << std::endl;
	}
	else
	{
		std::cout << "texture load failed!!" << std::endl;
	}
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, GLMODE, m_Width, m_Height, 0, GLMODE, GL_UNSIGNED_BYTE, m_LoadBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(m_LoadBuffer);

}

Texture::Texture(const Texture& t)
{
	int GLMODE = GL_RGBA;
	if (t.m_FilePath.find(".png") != std::string::npos)
		GLMODE = GL_RGBA;
	else if (t.m_FilePath.find(".jpg") != std::string::npos)
		GLMODE = GL_RGB;
	m_FilePath = t.m_FilePath;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image
	stbi_set_flip_vertically_on_load(true);
	m_LoadBuffer = stbi_load(t.m_FilePath.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);
	if (m_LoadBuffer != nullptr) {
		std::cout << "texture load successfully" << std::endl;
	}
	else
	{
		std::cout << "texture load failed!!" << std::endl;
	}
	//assert(m_LoadBuffer != nullptr);
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, GLMODE, m_Width, m_Height, 0, GLMODE, GL_UNSIGNED_BYTE, m_LoadBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(m_LoadBuffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Assign(const std::string& path)
{
	if (m_RendererID != 0) glDeleteTextures(1, &m_RendererID);
	m_FilePath = path;
	int GLMODE = GL_RGBA;
	if (path.find(".png") != std::string::npos)
		GLMODE = GL_RGBA;
	else if (path.find(".jpg") != std::string::npos)
		GLMODE = GL_RGB;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	m_LoadBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);
	if (m_LoadBuffer != nullptr) {
		std::cout << "texture load successfully" << std::endl;
	}
	else
	{
		std::cout << "texture load failed!!" << std::endl;
	}
	//assert(m_LoadBuffer != nullptr);
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, GLMODE, m_Width, m_Height, 0, GLMODE, GL_UNSIGNED_BYTE, m_LoadBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(m_LoadBuffer);
}


void Texture::Bind(unsigned int slot /*= 0*/)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
