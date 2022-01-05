#include "VertexArray.h"

#include "GL/glew.h"

VertexArray::VertexArray() 
	: mVertexBuffer(), mIndexBuffer() {

	glGenVertexArrays(1, &mRendererID);
}

void VertexArray::bind() const {
	glBindVertexArray(mRendererID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

void VertexArray::setVertexBuffer(const VertexBuffer& vertexBuffer) {
	glBindVertexArray(mRendererID);

	vertexBuffer.bind();
	mVertexBuffer = vertexBuffer;

	vertexBuffer.getAttribs().bind();

	glBindVertexArray(0);
	vertexBuffer.unbind();
}

void VertexArray::setIndexBuffer(const IndexBuffer& indexBuffer) {
	glBindVertexArray(mRendererID);

	indexBuffer.bind();
	mIndexBuffer = indexBuffer;

	glBindVertexArray(0);
	indexBuffer.unbind();
}