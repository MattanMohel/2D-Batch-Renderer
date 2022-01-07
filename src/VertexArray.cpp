#include "VertexArray.h"

#include "GL/glew.h"

VertexArray::VertexArray() 
	: mVertexBuffer(), mIndexBuffer() {

	glGenVertexArrays(1, &mRendererID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &mRendererID);
}

void VertexArray::bind() const {
	glBindVertexArray(mRendererID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

void VertexArray::setIndexBuffer(const IndexBuffer& indexBuffer) {
	glBindVertexArray(mRendererID);

	mIndexBuffer = indexBuffer;
	mIndexBuffer.bind();

	glBindVertexArray(0);
	mIndexBuffer.unbind();
}

void VertexArray::setVertexBuffer(const VertexBuffer& vertexBuffer, const BufferLayout& bufferLayout) {
	glBindVertexArray(mRendererID);

	mVertexBuffer = vertexBuffer;
	mBufferLayout = bufferLayout;
	
	mVertexBuffer.bind();
	mBufferLayout.bind();

	glBindVertexArray(0);
	mVertexBuffer.unbind();
}