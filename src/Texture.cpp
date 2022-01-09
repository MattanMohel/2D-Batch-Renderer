#include "Texture.h"

#include <GL/glew.h>

#include "stb_image/stb_image.h"

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
	glBindTextureUnit(slot, mRendererID);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::textureSlotCount() {
	int textureSlots;

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureSlots);
	return textureSlots;
}

uint32_t Texture::createTexture(const std::string& texturePath) {

	int width, height, bitsPerPixel;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* buffer = stbi_load(texturePath.c_str(), &width, &height, &bitsPerPixel, /*RGBA*/4);

	uint32_t textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /*GL_NEAREST - pixel perfect*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(buffer);

	return textureID;
}