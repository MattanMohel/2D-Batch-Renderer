#pragma once

#include "VertexArray.h"
#include "Attribute.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "Camera.h"

#define DEBUG_GL 1
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

class Pipeline {

public:
	static void init();

	static void draw(size_t vertexCount, size_t indexCount);

	static void endFrame();

	static int getTextureSlots() { return textureSlots; }
	static int getVertexCount()  { return vertexCount; }
	static int getdrawCalls()    { return drawCalls; }

private:
	static int textureSlots;
	static int vertexCount;
	static int drawCalls;
};

class Renderer2D {

public:
	void init(Camera* camera, const Shader& shader);

	void pushQuad(const glm::mat4& model, const glm::vec4& color, uint32_t texID);
	void flush();

	void drawBatch();

private:
	uint32_t m_Vbo, m_Ibo, m_Vao;

	Vertex  m_Vertices[4 * MAX_BATCH_QUAD_COUNT];
	uint32_t m_Indices[6 * MAX_BATCH_QUAD_COUNT];

	int m_TextureUniform[MAX_BATCH_QUAD_COUNT];

	uint32_t m_BatchIndex = 0, m_TexIndex = 0;

	Shader  m_Shader;
	Camera* m_Camera;
};

