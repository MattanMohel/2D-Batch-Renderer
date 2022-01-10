#pragma once

#include "glm/glm.hpp"

struct Vertex {
	glm::vec2 pos;
	glm::vec4 col;
	uint32_t tex;
	glm::vec2 UVs;
};