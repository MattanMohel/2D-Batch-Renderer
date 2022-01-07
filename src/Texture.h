#pragma once

#include <cstdint>
#include <string>

class Texture {

public:
	Texture(const std::string& texturePath);
	~Texture();

	void bind(uint32_t slot = 0) const;
	void unbind() const;

	static int textureSlotCount();

private:
	uint32_t mRendererID = -1;

	std::string mFilePath;
	unsigned char* mBuffer = nullptr;
	int mWidth = 0, mHeight = 0, mBitsPerPixel = 0;
};

