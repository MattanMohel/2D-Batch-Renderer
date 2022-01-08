#pragma once

#include <cstdint>
#include <string>

class Texture {

public:
	Texture(const std::string& texturePath);
	~Texture();

	void bind(uint32_t slot = 0) const;
	void unbind() const;

	uint32_t getRendererID() const { return mRendererID; }

	static int textureSlotCount();

	static uint32_t loadTexture(const std::string& texturePath);

private:
	uint32_t mRendererID = -1;

	std::string mFilePath;
	unsigned char* mBuffer = nullptr;
	int mWidth = 0, mHeight = 0, mBitsPerPixel = 0;
};

