#pragma once

#include <iostream>
#include <vector>

enum class GLtype {
	BYTE = 0x1400,
	BOOL = 0x8B56,
	UINT = 0x1405,
	INT = 0x1404,
	FLOAT = 0x1406,
};

struct BufferAttrib {
	GLtype type;
	uint32_t count;
	bool normalized;
	uint32_t offset;

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

