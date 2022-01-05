#include "Attribute.h"

#include <GL/glew.h>

BufferLayout::BufferLayout(const std::initializer_list<BufferAttrib>& attribs)
	: mAttribs(attribs) {

	mVertexStride = 0;

	for (auto& elem : mAttribs) {
		elem.offset = mVertexStride;
		mVertexStride += elem.count * elem.size();
	}
}

void BufferLayout::setAttribs(const std::initializer_list<BufferAttrib>& attribs) {
	mAttribs = attribs;

	mVertexStride = 0;

	for (auto& elem : mAttribs) {
		elem.offset = mVertexStride;
		mVertexStride += elem.count * elem.size();
	}
}

uint32_t BufferLayout::getStride() const {
	return mVertexStride;
}

std::vector<BufferAttrib>& BufferLayout::getAttribs() {
	return mAttribs;
}

void BufferLayout::bind() const {
	uint32_t attribIndex = 0;

	for (auto& elem : mAttribs) {
		glEnableVertexAttribArray(attribIndex);
		glVertexAttribPointer(attribIndex,elem.count, (uint32_t)elem.type, elem.normalized,
			/*stride of vertex in bytes*/mVertexStride, 
			/*offset of attrib in bytes*/(const void*)elem.offset);

		++attribIndex;
	}
}