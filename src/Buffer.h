#pragma once

#include <cstdint>

class VertexBuffer {

public:
	VertexBuffer() = default;

	VertexBuffer(float* vertices, uint32_t size);
	~VertexBuffer();

	void setBuffer(float* vertices, uint32_t size);

	void bind() const;
	void unbind() const;

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
	void unbind() const;

private:
	uint32_t mRendererID = -1;
	uint32_t mIndexCount;
};

