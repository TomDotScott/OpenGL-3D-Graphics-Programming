#include "Mesh.h"

Mesh::Mesh(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, Vertex* vertexArray,
	const unsigned& numOfVertices, GLuint* indexArray, const unsigned& numOfIndices)
:
	m_numVertices(numOfVertices),
	m_numIndices(numOfIndices),
	m_vao(0),
	m_vbo(0),
	m_ebo(0),
	m_position(position),
	m_rotation(rotation),
	m_scale(scale),
	m_modelMatrix(glm::mat4(1.f))
{
	InitialiseBuffers(vertexArray, indexArray);
}

Mesh::Mesh(const glm::vec3&     position, const glm::vec3& rotation, const glm::vec3& scale,
           const ePrimitiveType type)
:
	m_numVertices(0),
	m_numIndices(0),
	m_vao(0),
	m_vbo(0),
	m_ebo(0),
	m_position(position),
	m_rotation(rotation),
	m_scale(scale),
	m_modelMatrix(glm::mat4(1.f))
{
	Primitive* primitive = nullptr;
	
	switch (type)
	{
		case ePrimitiveType::e_Quad:
			primitive = new Quad();
			break;
		case ePrimitiveType::e_Triangle: 
			primitive = new Triangle();
			break;
		case ePrimitiveType::e_Pyramid:
			primitive = new Pyramid();
			break;
		case ePrimitiveType::e_Cube:
			primitive = new Cube();
			break;
		default: 
			break;
	}

	if (primitive)
	{
		InitialiseBuffers(*primitive);

		m_numVertices = primitive->GetVertices().size();
		m_numIndices = primitive->GetIndices().size();
	}else
	{
		std::cout << "Error creating primitive. Check type" << std::endl;
	}
	
	delete primitive;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	
	glDeleteBuffers(1, &m_vbo);
	
	if (m_numIndices > 0)
	{
		glDeleteBuffers(1, &m_ebo);
	}
}

void Mesh::Render(Shader& shader)
{
	UpdateModelMatrix();
	UpdateUniforms(shader);

	//Bind vertex array object
	glBindVertexArray(m_vao);

	shader.Use();

	//Draw
	if (m_numIndices == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	} else
	{
		glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
	}

	shader.Unuse();
}

void Mesh::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

void Mesh::SetRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
}

void Mesh::SetScale(const glm::vec3& scale)
{
	m_scale = scale;
}

glm::vec3 Mesh::GetPosition() const
{
	return m_position;
}

glm::vec3 Mesh::GetRotation() const
{
	return m_rotation;
}

glm::vec3 Mesh::GetScale() const
{
	return m_scale;
}

void Mesh::InitialiseBuffers(Vertex* vertexArray, GLuint* indexArray)
{
	//VAO, VBO, EBO
	glCreateVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

	// We only want to generate an elements array if there are indices passed in...
	if (m_numIndices > 0)
	{
		//GEN EBO AND BIND AND SEND DATA
		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);
	}

	//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<GLvoid*>(offsetof(Vertex, m_position)));
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<GLvoid*>(offsetof(Vertex, m_colour)));
	glEnableVertexAttribArray(1);
	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<GLvoid*>(offsetof(Vertex, m_texcoord)));
	glEnableVertexAttribArray(2);
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<GLvoid*>(offsetof(Vertex, m_normal)));
	glEnableVertexAttribArray(3);

	//BIND VAO 0
	glBindVertexArray(0);
}

void Mesh::InitialiseBuffers(Primitive& primitive)
{
	m_numVertices = primitive.GetVertices().size();
	m_numIndices = primitive.GetIndices().size();

	//VAO, VBO, EBO
	glCreateVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vertex), primitive.GetVertices().data(), GL_STATIC_DRAW);

	if (m_numIndices > 0)
	{
		//GEN EBO AND BIND AND SEND DATA
		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), primitive.GetIndices().data(), GL_STATIC_DRAW);
	}

	//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<GLvoid*>(offsetof(Vertex, m_position)));
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<GLvoid*>(offsetof(Vertex, m_colour)));
	glEnableVertexAttribArray(1);
	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<GLvoid*>(offsetof(Vertex, m_texcoord)));
	glEnableVertexAttribArray(2);
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<GLvoid*>(offsetof(Vertex, m_normal)));
	glEnableVertexAttribArray(3);

	//BIND VAO 0
	glBindVertexArray(0);
}

void Mesh::UpdateUniforms(Shader& shader) const
{
	shader.SetMat4Fv(m_modelMatrix, "model_matrix");
}

void Mesh::UpdateModelMatrix()
{
	m_modelMatrix = glm::mat4(1.f);
	m_modelMatrix = translate(m_modelMatrix, m_position);
	m_modelMatrix = rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));
	m_modelMatrix = rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
	m_modelMatrix = rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}
