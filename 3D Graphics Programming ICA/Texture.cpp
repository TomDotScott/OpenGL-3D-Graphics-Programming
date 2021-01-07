#include "Texture.h"

#include <iostream>
#include <SOIL2/SOIL2.h>

Texture::Texture(const std::string& fileName, const GLenum type, const GLint textureUnit) :
	m_ID(0),
	m_width(0),
	m_height(0),
	m_type(type),
	m_textureUnit(textureUnit)
{
	unsigned char* image = SOIL_load_image(fileName.c_str(), &m_width, &m_height, nullptr, SOIL_LOAD_RGBA);

	glGenTextures(1, &m_ID);
	glBindTexture(type, m_ID);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(type, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(type);
	} else
	{
		std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
	}

	glActiveTexture(0);
	glBindTexture(type, 0);
	SOIL_free_image_data(image);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

GLuint Texture::GetID() const
{
	return m_ID;
}

void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_textureUnit);
	glBindTexture(m_type, m_ID);
}

void Texture::Unbind() const
{
	glActiveTexture(0);
	glBindTexture(m_type, 0);
}

void Texture::LoadFromFile(const std::string& fileName)
{
	if (m_ID)
	{
		glDeleteTextures(1, &m_ID);
	}

	unsigned char* image = SOIL_load_image(fileName.c_str(), &m_width, &m_height, nullptr, SOIL_LOAD_RGBA);

	glGenTextures(1, &m_ID);
	glBindTexture(m_type, m_ID);

	glTexParameteri(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(m_type, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(m_type);
	} else
	{
		std::cout << "ERROR::TEXTURE::LOADFROMFILE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
	}

	glActiveTexture(0);
	glBindTexture(m_type, 0);
	SOIL_free_image_data(image);
}
