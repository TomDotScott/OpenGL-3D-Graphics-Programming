#include "Material.h"

Material::Material(const glm::vec3& ambientColour, const glm::vec3& diffuseColour, const glm::vec3& specularColour,
	const GLint diffuseTexture, const GLint specularTexture) :
	m_ambientColour(ambientColour),
	m_diffuseColour(diffuseColour),
	m_specularColour(specularColour),
	m_diffuseTexture(diffuseTexture),
	m_specularTexture(specularTexture)
{
}

void Material::SendToShader(Shader& program) const
{
	program.SetVec3F(m_ambientColour, "material.ambient");
	program.SetVec3F(m_diffuseColour, "material.diffuse");
	program.SetVec3F(m_specularColour, "material.specular");
	program.Set1I(m_diffuseTexture, "material.diffuse_tex");
	program.Set1I(m_specularTexture, "material.specular_tex");
}
