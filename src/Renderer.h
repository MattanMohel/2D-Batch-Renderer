#pragma once

#include "VertexArray.h"
#include "Attribute.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

#define DEBUG_GL 0
#define MAX_BATCH_QUAD_COUNT 1000

static constexpr glm::vec2 vertices[] = {
	glm::vec2(-1.0f, -1.0f),
	glm::vec2(-1.0f,  1.0f),
	glm::vec2( 1.0f,  1.0f),
	glm::vec2( 1.0f, -1.0f),
};

static constexpr glm::vec2 texCoords[] = {
	glm::vec2(0.0f, 0.0f),
	glm::vec2(0.0f, 1.0f),
	glm::vec2(1.0f, 1.0f),
	glm::vec2(1.0f, 0.0f),
};

class Renderer {

public:
	static void initGLEW();

	void initBatching();

	void pushQuad(const glm::mat4& model, const glm::vec4& color, uint32_t texID);
	void flush();

	uint32_t queryFlushCount();

	void drawBatch();

	static void draw(const VertexArray& vertexArray, const Shader& shader);

	Shader mShader;
	glm::mat4 m_ViewProjection;

private:
	uint32_t mVertexBufferID = -1;
	uint32_t mVertexArrayID  = -1;
	uint32_t mIndexBufferID  = -1;

	Vertex  mBatchVertexBuffer[4 * MAX_BATCH_QUAD_COUNT];
	uint32_t mBatchIndexBuffer[6 * MAX_BATCH_QUAD_COUNT];

	float mMatrixArray[16 * MAX_BATCH_QUAD_COUNT];
	int   mTextureArray[MAX_BATCH_QUAD_COUNT];

	uint32_t mBufferIndex = 0;
	uint32_t mTextureIndex = 0;
	uint32_t mFlushCount = 0;

	static uint32_t maxTextureSlots;
};

