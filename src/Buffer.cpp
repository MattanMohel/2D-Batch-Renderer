#include "Buffer.h"

#include <GL/glew.h>

namespace vertexBuffer {

	uint32_t create() {
		uint32_t id;
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);

		return id;
	}

	void destroy(uint32_t id) {
		glDeleteBuffers(1, &id);
	}

	void setBuffer(const void* vertices, uint32_t count, 
		uint32_t size, gl::type drawHint) {

		glBufferData(GL_ARRAY_BUFFER, count * size, vertices, (int)drawHint);
	}

	void bind(uint32_t id) {
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

namespace attributes {

	size_t create(uint32_t index, int count, size_t stride, size_t offset,
		gl::type type, bool normalized) {

		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, count, (int)type, normalized, stride, (const void*)offset);

		return offset + count * gl::size(type);
	}
}

namespace indexBuffer {

	uint32_t create() {
		uint32_t id;
		glGenBuffers(1, &id);

		return id;
	}

	void destroy(uint32_t id) {
		glDeleteBuffers(1, &id);
	}

	void setBuffer(const void* indices, uint32_t count, 
		uint32_t size, gl::type drawHint) {

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * size, indices, (int)drawHint);
	}

	void bind(uint32_t id) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

namespace vertexArray {

	uint32_t create() {
		uint32_t id;
		glGenVertexArrays(1, &id);

		return id;
	}

	void destroy(uint32_t id) {
		glDeleteVertexArrays(1, &id);
	}

	void bind(uint32_t id) {
		glBindVertexArray(id);
	}

	void unbind() {
		glBindVertexArray(0);
	}
}