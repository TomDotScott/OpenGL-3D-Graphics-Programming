#pragma once
#include <vector>
#include <gl/glew.h>

#include "Vertex.h"

class Primitive
{
public:
	Primitive() = default;

	virtual ~Primitive() = default;

	void Set(const Vertex* vertices, const unsigned numOfVertices,
	         const GLuint* indices, const unsigned  numOfIndices)
	{
		for (unsigned i = 0; i < numOfVertices; ++i)
		{
			m_vertices.emplace_back(vertices[i]);
		}

		for (unsigned i = 0; i < numOfIndices; ++i)
		{
			m_indices.emplace_back(indices[i]);
		}
	}

	std::vector<Vertex>& GetVertices()
	{
		return m_vertices;
	}

	std::vector<GLuint>& GetIndices()
	{
		return m_indices;
	}

private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
};

class Quad final : public Primitive
{
public:
	Quad() : Primitive()
	{
		Vertex vertices[] =
		{
			//Position								//Color							//Texcoords					//Normals
			{glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f)},
			{glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f)},
			{glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)},
			{glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, 1.f)}
		};

		const unsigned numOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2, //Triangle 1
			0, 2, 3  //Triangle 2
		};
		const unsigned numOfIndices = sizeof(indices) / sizeof(GLuint);

		Set(vertices, numOfVertices, indices, numOfIndices);
	}
};

class Triangle final : public Primitive
{
public:
	Triangle() : Primitive()
	{
		Vertex vertices[] =
		{
			//Position								   //Color							     //Texcoords					//Normals
			{glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f)},
			{glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f)},
			{glm::vec3(0.f, 0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)},
		};

		const unsigned numOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2, //Triangle 1
		};
		const unsigned numOfIndices = sizeof(indices) / sizeof(GLuint);

		Set(vertices, numOfVertices, indices, numOfIndices);
	}
};
