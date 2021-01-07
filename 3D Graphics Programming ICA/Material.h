#pragma once
#include <gl/glew.h>
#include <glm/vec3.hpp>

#include "Shader.h"

class Material
{
public:
	Material(const glm::vec3& ambientColour, const glm::vec3& diffuseColour, const glm::vec3& specularColour, const GLint diffuseTexture, const GLint specularTexture);

	void SendToShader(Shader& program) const;

private:
	glm::vec3 m_ambientColour;
	glm::vec3 m_diffuseColour;
	glm::vec3 m_specularColour;
	GLint m_diffuseTexture;
	GLint m_specularTexture;
};
