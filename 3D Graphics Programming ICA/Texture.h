#pragma once
#include<string>
#include <gl/glew.h>

class Texture
{
public:
	Texture(const std::string& fileName, const GLenum type);

	~Texture();

	inline GLuint GetID() const;

	void Bind(GLint textureUnit) const;

	void Unbind() const;

	void LoadFromFile(const std::string& fileName);

private:
	GLuint m_ID;
	int m_width;
	int m_height;
	unsigned int m_type;
};
