#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "GLtype.h"

struct BufferAttrib {
	GLtype type = GLtype::FLOAT;
	uint32_t count = 0;
	bool normalized = false;
	uint32_t offset = 0;

	uint32_t size() const {
		switch (type) {
		case GLtype::BYTE:
		case GLtype::BOOL:
			return 1;
		case GLtype::UINT:
		case GLtype::INT:
		case  GLtype::FLOAT:
			return 4;
		}

		return 0;
	}
};

class BufferLayout {

public:
	BufferLayout() = default;

	BufferLayout(const std::initializer_list<BufferAttrib>& attribs);

	void setAttribs(const std::initializer_list<BufferAttrib>& attribs);

	uint32_t getStride() const;

	std::vector<BufferAttrib>& getAttribs();

	void bind() const;

private:
	std::vector<BufferAttrib> mAttribs;
	uint32_t mVertexStride;
};

