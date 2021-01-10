#pragma once
#include<fstream>
#include<iostream>
#include<string>

#include <gl/glew.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(const int glVersionMajor, 
		const int glVersionMinor, 
		const std::string& vertexFile, 
		const std::string& fragmentFile, 
		const std::string& geometryFile = ""
	);

	~Shader();
	
	void Use();

	void Unuse();

	void Set1I(GLint value, const std::string& name);

	void Set1F(GLfloat value, const std::string& name);

	void SetVec2F(glm::fvec2 value, const std::string& name);

	void SetVec3F(glm::fvec3 value, const std::string& name);

	void SetVec4F(glm::fvec4 value, const std::string& name);

	void SetMat3Fv(glm::mat3 value, const std::string& name, GLboolean transpose = GL_FALSE);

	void SetMat4Fv(glm::mat4 value, const std::string& name, GLboolean transpose = GL_FALSE);

private:
	GLuint m_ID;
	const int m_glVersionMajor;
	const int m_glVersionMinor;

	std::string LoadShaderSource(const std::string& fileName) const;
	GLuint LoadShader(GLenum type, const std::string& fileName) const;
	void LinkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);
};