#pragma once

#include "glm/glm.hpp"

class Transform {

public:
	Transform();

	glm::vec2& position() { return m_Position; }
	glm::vec2& scale()    { return m_Scale; }
	float&     rotation() { return m_Rotation; }

	glm::mat4 modelMatrix() const;

private:
	glm::vec2 m_Position;
	glm::vec2 m_Scale;
	float m_Rotation;
};

