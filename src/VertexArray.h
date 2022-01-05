#pragma once

#include "Buffer.h"
#include "Attribute.h"

class VertexArray {

public:
	VertexArray();

	void bind() const;
	void unbind() const;

	void setVertexBuffer(const VertexBuffer& vertexBuffer);
	void setIndexBuffer(const IndexBuffer& indexBuffer);


private:
	uint32_t  mRendererID;

	VertexBuffer mVertexBuffer;
	IndexBuffer   mIndexBuffer;
};

