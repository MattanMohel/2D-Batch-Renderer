#pragma once

#include "Buffer.h"
#include "Attribute.h"

class VertexArray {

public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;

	void setIndexBuffer(const IndexBuffer& indexBuffer);
	void setVertexBuffer(const VertexBuffer& vertexBuffer, const BufferLayout& layout);

private:
	uint32_t  mRendererID = -1;

	VertexBuffer mVertexBuffer;
	BufferLayout mBufferLayout;
	IndexBuffer   mIndexBuffer;
};

