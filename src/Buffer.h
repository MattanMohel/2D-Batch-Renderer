#pragma once

#include <vector>
#include "GLtype.h"

namespace vertexBuffer {
	uint32_t create();
	void destroy(uint32_t id);

	void setBuffer(const void* vertices, uint32_t count, 
		uint32_t size, gl::type drawHint = gl::type::STATIC_DRAW);

	void bind(uint32_t id);
	void unbind();
}

namespace attributes {
	// returns offset for next attribute
	size_t create(uint32_t index, int count, size_t stride, size_t offset, 
		gl::type type = gl::type::FLOAT, bool normalized = false);
}

namespace indexBuffer {
	uint32_t create();
	void destroy(uint32_t id);

	void setBuffer(const void* indices, uint32_t count, 
		uint32_t size, gl::type drawHint = gl::type::STATIC_DRAW);

	void bind(uint32_t id);
	void unbind();
}

namespace vertexArray {
	uint32_t create();
	void destroy(uint32_t id);

	void bind(uint32_t id);
	void unbind();
}