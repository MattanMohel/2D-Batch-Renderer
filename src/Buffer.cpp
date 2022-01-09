#include "Buffer.h"

#include <GL/glew.h>

///////////////////////
/////Vertex Buffer/////
///////////////////////

VertexBuffer::VertexBuffer(float* vertices, uint32_t size) {
	glGenBuffers(1, &mRendererID);
	glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &mRendererID);
}

void VertexBuffer::setBuffer(float* vertices, uint32_t size) {
	glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
}

void VertexBuffer::bind(uint32_t id) {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

uint32_t VertexBuffer::create(const void* vertices, uint32_t count, uint32_t itemSize, GLtype drawHint) {
	uint32_t rendererID;

	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ARRAY_BUFFER, count * itemSize, vertices, (uint32_t)drawHint);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return rendererID;
}

//////////////////////
/////Index Buffer/////
//////////////////////

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t size) 
	: mIndexCount(size) {

	glGenBuffers(1, &mRendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &mRendererID);
}

void IndexBuffer::setBuffer(uint32_t* indices, uint32_t size) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mIndexCount = size;
}

void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void IndexBuffer::bind(uint32_t id) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t IndexBuffer::create(uint32_t* indices, uint32_t size, GLtype drawHint) {
	uint32_t rendererID;

	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), indices, (uint32_t)drawHint);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return rendererID;
}