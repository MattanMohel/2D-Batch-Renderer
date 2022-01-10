#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

void Camera::setProjection(float hrzRadius, float vrtRadius) {
	m_Projection = glm::ortho(-hrzRadius, hrzRadius, -vrtRadius, vrtRadius);
}

void Camera::setView(const glm::vec2& position) {
	m_View - glm::translate(glm::mat4(1.0f), -glm::vec3(position, 1.0f));
}

void Camera::updateViewProjection() {
	m_ViewProjection = m_Projection * m_View;
}