#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
	Vertex(const glm::vec3& pos, const glm::vec3& col, const glm::vec2& uvs, const glm::vec3& normals) :
		m_position(pos),
		m_colour(col),
		m_texcoord(uvs),
		m_normal(normals)
	{
	}

	glm::vec3 m_position;
	glm::vec3 m_colour;
	glm::vec2 m_texcoord;
	glm::vec3 m_normal;
};
