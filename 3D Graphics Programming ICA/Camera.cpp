#include "Camera.h"
Camera::Camera(const glm::vec3& position, const glm::vec3& up) :
	m_position(position),
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_up(up),
	m_right(0.f),
	m_worldUp(up),
	m_yaw(-90.f),
	m_pitch(0.0f),
	m_movementSpeed(2.5f),
	m_mouseSensitivity(0.1f)
{
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

const glm::vec3& Camera::GetPosition() const
{
	return m_position;
}

void Camera::Move(const float deltaTime, const eDirection direction)
{
	const float velocity = m_movementSpeed * deltaTime;
	if (direction == eDirection::e_Forward)
		m_position += m_front * velocity;
	if (direction == eDirection::e_Backward)
		m_position -= m_front * velocity;
	if (direction == eDirection::e_Left)
		m_position -= m_right * velocity;
	if (direction == eDirection::e_Right)
		m_position += m_right * velocity;
}

void Camera::ProcessMouseMovement(const float xOffset, const float yOffset, const GLboolean constrainPitch)
{
	m_yaw += xOffset * m_mouseSensitivity;
	m_pitch += yOffset * m_mouseSensitivity;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

void Camera::Update(const float xOffset, const float yOffset, const GLboolean constrainPitch)
{
	ProcessMouseMovement(xOffset, yOffset, constrainPitch);
	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 temp(0.f);
	temp.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	temp.y = sin(glm::radians(m_pitch));
	temp.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(temp);

	// also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));

	// normalize the vectors, because their length gets closer to 0 the more you
	// look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
