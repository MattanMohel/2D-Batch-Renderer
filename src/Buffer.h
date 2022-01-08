#pragma once

#include <cstdint>
#include "GLtype.h"
#include "Vertex.h"

class VertexBuffer {

public:
	VertexBuffer() = default;

	VertexBuffer(float* vertices, uint32_t size);
	~VertexBuffer();

	void setBuffer(float* vertices, uint32_t size);

	void bind() const;
	static void bind(uint32_t id);

	static void unbind();	
	
	static uint32_t createVertexBuffer(const void* vertices, uint32_t count, uint32_t itemSize, GLtype drawHint);

private:
	uint32_t  mRendererID = -1;
};

class IndexBuffer {

public:
	IndexBuffer() = default;

	IndexBuffer(uint32_t* indices, uint32_t size);
	~IndexBuffer();

	void setBuffer(uint32_t* indices, uint32_t size);

	void bind() const;
	static void bind(uint32_t id);

	static void unbind();

	static uint32_t createIndexBuffer(uint32_t* indices, uint32_t size, GLtype drawHint);

private:
	uint32_t mRendererID = -1;
	uint32_t mIndexCount;
};

