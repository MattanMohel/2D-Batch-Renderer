#pragma once

namespace gl {

	// Contained definitions for OpenGL type values
	enum class type {
		BYTE = 0x1400,
		BOOL = 0x8B56,
		UINT = 0x1405,
		INT = 0x1404,
		FLOAT = 0x1406,

		FRAGMENT_SH = 0x8B30,
		VERTEX_SH = 0x8B31,
		COMPUTE_SH = 0x91B9,
		GEOMETRY_SH = 0x8DD9,

		STATIC_DRAW = 0x88E4,
		DYNAMIC_DRAW = 0x88E8,
	};

	static size_t size(type type) {
		switch (type) {
		case type::BYTE:
		case type::BOOL:
			return 1;
		case type::UINT:
		case type::INT:
		case type::FLOAT:
			return 4;
		}

		return 0;
	}
};