#include "Texture.h"

#include <GL/glew.h>

#include "vendor/stb_image.h"

Texture::Texture(const std::string& texturePath)
	: mFilePath(texturePath) {

	stbi_set_flip_vertically_on_load(true);
	mBuffer = stbi_load(texturePath.c_str(), &mWidth, &mHeight, &mBitsPerPixel, /*RGBA*/4);

	glGenTextures(1, &mRendererID);
	glBindTexture(GL_TEXTURE_2D, mRendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /*GL_NEAREST - pixel perfect*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
	glDeleteTextures(1, &mRendererID);

	if (mBuffer) {
		stbi_image_free(mBuffer);
	}
}

void Texture::bind(uint32_t slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mRendererID);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::textureSlotCount() {
	static int textureSlots = -1;

	if (textureSlots != -1) {
		return textureSlots;
	}

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureSlots);
	return textureSlots;
}