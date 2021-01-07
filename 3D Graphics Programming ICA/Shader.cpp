#include "Shader.h"
Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile, const std::string& geometryFile)
{
	GLuint geometryShader = 0;

	const GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertexFile);

	if (!geometryFile.empty())
		geometryShader = LoadShader(GL_GEOMETRY_SHADER, geometryFile);

	const GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentFile);

	this->LinkProgram(vertexShader, geometryShader, fragmentShader);

	//End
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::Use()
{
	glUseProgram(this->m_ID);
}

void Shader::Unuse()
{
	glUseProgram(0);
}

void Shader::Set1I(const GLint value, const std::string& name) {
	Use();

	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);

	Unuse();
}

void Shader::Set1F(const GLfloat value, const std::string& name)
{
	Use();

	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);

	Unuse();
}

void Shader::SetVec2F(glm::fvec2 value, const std::string& name)
{
	Use();

	glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));

	Unuse();
}

void Shader::SetVec3F(glm::fvec3 value, const std::string& name)
{
	Use();

	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));

	Unuse();
}

void Shader::SetVec4F(glm::fvec4 value, const std::string& name)
{
	Use();

	glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));

	Unuse();
}

void Shader::SetMat3Fv(glm::mat3 value, const std::string& name, const GLboolean transpose)
{
	Use();

	glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, transpose, glm::value_ptr(value));

	Unuse();
}

void Shader::SetMat4Fv(glm::mat4 value, const std::string& name, GLboolean transpose)
{
	Use();

	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, transpose, glm::value_ptr(value));

	Unuse();
}

std::string Shader::LoadShaderSource(const std::string& fileName) const
{
	std::ifstream inFile;

	//Vertex
	inFile.open(fileName);

	std::string temp;
	std::string src;

	if (inFile.is_open())
	{
		while (std::getline(inFile, temp))
			src += temp + "\n";
	} else
	{
		std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << fileName << "\n";
	}

	inFile.close();

	return src;
}

GLuint Shader::LoadShader(const GLenum type, const std::string& fileName) const
{
	char infoLog[512];
	GLint success;

	const GLuint      shader = glCreateShader(type);
	const std::string source = LoadShaderSource(fileName);
	const GLchar* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
		std::cout << infoLog << "\n";
	}

	return shader;
}

void Shader::LinkProgram(const GLuint vertexShader, const GLuint geometryShader, const GLuint fragmentShader)
{
	char infoLog[512];
	GLint success;

	m_ID = glCreateProgram();

	glAttachShader(m_ID, vertexShader);

	if (geometryShader)
		glAttachShader(m_ID, geometryShader);

	glAttachShader(m_ID, fragmentShader);

	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
		std::cout << infoLog << "\n";
	}

	glUseProgram(0);
}