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

void VertexBuffer::setBuffer(float* vertices, uint32_t size) {
	glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
}

void VertexBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void IndexBuffer::setBuffer(uint32_t* indices, uint32_t size) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mIndexCount = size;
}

void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}
void IndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


VertexAttrib::VertexAttrib(const std::initializer_list<VertexAttribElem>& elems) 
	: mElems(elems) {

	uint32_t offset = 0;

	for (auto& elem : mElems) {
		elem.offset = offset;
		offset += elem.count * sizeof(float); //TODO: not always a float
	}
}

void VertexAttrib::bind() const {
	uint32_t attribIndex = 0;
	
	for (auto& elem : mElems) {
		glEnableVertexAttribArray(attribIndex);
		glVertexAttribPointer(attribIndex, elem.count, (uint32_t)elem.type, elem.normalized, elem.count * elem.size(), (const void*)elem.offset);
	}
}