#include "Texture.h"

#include <GL/glew.h>

#include "stb_image/stb_image.h"

namespace texture {

	uint32_t create(const std::string& path) {
		stbi_set_flip_vertically_on_load(true);

		int width, height, bpp;

		unsigned char* buffer = stbi_load(path.c_str(), &width, &height, &bpp, /*RGBA*/4);

		uint32_t id;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(buffer);

		return id;
	}

	uint32_t create(int width, int height, uint32_t* texBuffer) {
		stbi_set_flip_vertically_on_load(true);

		uint32_t id;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		return id;
	}

	void destroy(uint32_t id) {
		glDeleteTextures(1, &id);
	}

	void bind(uint32_t id, uint32_t slot) {
		glBindTextureUnit(slot, id);
	}

	void unbind(uint32_t slot) {
		glBindTextureUnit(slot, 0);
	}
}