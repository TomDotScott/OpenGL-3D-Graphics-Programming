#pragma once
#include "Vertex.h"
#include <vector>
#include <gl/glew.h>


#include "Primitives.h"
#include "Shader.h"

enum class ePrimitiveType
{
	e_Quad, e_Triangle, e_Pyramid, e_Cube
};

class Mesh
{
public:
	Mesh(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale,
		Vertex* vertexArray, const unsigned& numOfVertices,
		GLuint* indexArray, const unsigned& numOfIndices);

	Mesh(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, ePrimitiveType type);
	
	~Mesh();

	void Render(Shader& shader);

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;

private:
	unsigned m_numVertices;
	unsigned m_numIndices;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_modelMatrix;

	void InitialiseBuffers(Vertex* vertexArray, GLuint* indexArray);
	
	void InitialiseBuffers(Primitive& primitive);

	void UpdateUniforms(Shader& shader) const;

	void UpdateModelMatrix();
};
