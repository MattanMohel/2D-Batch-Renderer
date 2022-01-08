#pragma once

#include "VertexArray.h"
#include "Attribute.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

#define DEBUG_GL 1
#define MAX_BATCH_QUAD_COUNT 2

class Renderer {

public:
	void init(bool initBatchBuffer = true);

	void pushQuad(const glm::mat4& mvp, const glm::vec4& color, uint32_t texID);
	void flush();

	void drawBatch(const Shader& shader);

	static void draw(const VertexArray& vertexArray, const Shader& shader);

private:
	uint32_t mVertexBufferID = -1;
	uint32_t mVertexArrayID  = -1;
	uint32_t mIndexBufferID  = -1;

	Vertex  mBatchVertexBuffer[4 * MAX_BATCH_QUAD_COUNT];
	uint32_t mBatchIndexBuffer[6 * MAX_BATCH_QUAD_COUNT];

	float mMatrixArray[16 * MAX_BATCH_QUAD_COUNT];
	int   mTextureArray[MAX_BATCH_QUAD_COUNT];

	uint32_t mBufferIndex = 0;
};

