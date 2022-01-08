#pragma once

#include "VertexArray.h"
#include "Attribute.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"

#define DEBUG_GL 1

class Renderer {

public:
	static void init();

	static void draw(const VertexArray& vertexArray, const Shader& shader);
};

