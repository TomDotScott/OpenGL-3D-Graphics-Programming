#pragma once
#include <iostream>
#include <gl/glew.h>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>

enum class eDirection
{
	e_None, e_Forward, e_Backward, e_Left, e_Right
};

class Camera
{
public:
	Camera(const glm::vec3& position, const glm::vec3& up);

	glm::mat4 GetViewMatrix() const;

	const glm::vec3& GetPosition() const;

	void Move(float deltaTime, eDirection direction);

	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

	void Update(float xOffset, float yOffset, GLboolean constrainPitch = true);

private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;
	float m_movementSpeed;
	float m_mouseSensitivity;

	// calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors();
};
