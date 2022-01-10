#pragma once

#include <string>

namespace texture {
	uint32_t create(const std::string& path);
	void destroy(uint32_t id);

	void bind(uint32_t id, uint32_t slot = 1);
	void unbind(uint32_t slot = 1);
}

