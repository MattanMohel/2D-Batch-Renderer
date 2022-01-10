#pragma once

#include <glm/glm.hpp>

class Camera {

public:
	void setProjection(float hrzRadius, float vrtRadius);
	void setView(const glm::vec2& position);

	void updateViewProjection();

	const glm::mat4& getViewProjection() { return m_ViewProjection; }

private:
	glm::mat4 m_Projection;
	glm::mat4 m_View;

	glm::mat4 m_ViewProjection;
};

