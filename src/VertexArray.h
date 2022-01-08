#pragma once

#include "Buffer.h"
#include "Attribute.h"

class VertexArray {

public:
	VertexArray();
	~VertexArray();

	void bind() const;
	static void bind(uint32_t id);

	static void unbind();

	void setIndexBuffer(const IndexBuffer& indexBuffer);
	void setVertexBuffer(const VertexBuffer& vertexBuffer, const BufferLayout& layout);

	static uint32_t createVertexArray();

private:
	uint32_t  mRendererID = -1;

	VertexBuffer mVertexBuffer;
	BufferLayout mBufferLayout;
	IndexBuffer   mIndexBuffer;
};

