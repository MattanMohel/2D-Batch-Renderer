#pragma once

#include <iostream>

#include "Attribute.h"

class VertexBuffer {

public:
	VertexBuffer() = default;

	VertexBuffer(float* vertices, uint32_t size);
	VertexBuffer(float* vertices, uint32_t size, const BufferLayout& attribs);
	~VertexBuffer();

	void setBuffer(float* vertices, uint32_t size);

	const BufferLayout& getAttribs() const;
	void setAttribs(const BufferLayout& attribs);

	void bind() const;
	void unbind() const;

private:
	uint32_t  mRendererID;
	BufferLayout mAttribs;
};

class IndexBuffer {

public:
	IndexBuffer() = default;

	IndexBuffer(uint32_t* indices, uint32_t size);
	~IndexBuffer();

	void setBuffer(uint32_t* indices, uint32_t size);

	void bind() const;
	void unbind() const;

private:
	uint32_t mRendererID;
	uint32_t mIndexCount;
};

