#pragma once

#include <iostream>
#include <vector>

enum class GLtype {
	BYTE = 0x1400,
	BOOL = 0x8B56,
	UINT = 0x1405,
	INT  = 0x1404,
	FLOAT = 0x1406,
};

class VertexBuffer {

public:
	VertexBuffer(float* vertices, uint32_t size);

	void setBuffer(float* vertices, uint32_t size);

	void bind() const;
	void unbind() const;

private:
	uint32_t mRendererID;
};

class IndexBuffer {

public:
	IndexBuffer(uint32_t* indices, uint32_t size);

	void setBuffer(uint32_t* indices, uint32_t size);

	void bind() const;
	void unbind() const;

private:
	uint32_t mRendererID;
	uint32_t mIndexCount;
};

struct VertexAttribElem {
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

class VertexAttrib {

public:
	VertexAttrib(const std::initializer_list<VertexAttribElem>& elems);

	void bind() const;

private:
	std::vector<VertexAttribElem> mElems;
};