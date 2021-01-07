#pragma once
#include<string>
#include <gl/glew.h>

class Texture
{
public:
	Texture(const std::string& fileName, const GLenum type, const GLint textureUnit);

	~Texture();

	inline GLuint GetID() const;

	void Bind() const;

	void Unbind() const;

	inline GLint GetTextureUnit() const { return m_textureUnit; }

	void LoadFromFile(const std::string& fileName);

private:
	GLuint m_ID;
	int m_width;
	int m_height;
	unsigned int m_type;
	GLint m_textureUnit;
};
