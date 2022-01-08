#pragma once

#include "glm/glm.hpp"

struct Vertex {
	glm::vec2 pos;
	glm::vec4 col;
	float tex;
	glm::vec2 UVs;
	float mvp;
};