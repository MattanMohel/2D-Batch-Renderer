#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
	: m_Position(0, 0), m_Scale(1, 1), m_Rotation(0) {}

glm::mat4 Transform::modelMatrix() const {
	glm::mat4 model(1.0f);
	model = glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));
	model = glm::translate(model, glm::vec3(m_Position, 1.0f));
	return glm::scale(model, glm::vec3(m_Scale, 0));
}