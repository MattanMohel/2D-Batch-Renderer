#pragma once

#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#define DEBUG_GL 0
#define MAX_BATCH_QUAD_COUNT 1000

#define END 1

namespace data {

	static constexpr
		glm::vec2 vertices[] = {
			glm::vec2(-1.0f, -1.0f),
			glm::vec2(-1.0f,  1.0f),
			glm::vec2(1.0f,  1.0f),
			glm::vec2(1.0f, -1.0f),
	};

	static constexpr
		glm::vec2 texCoords[] = {
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
	};

	struct Vertex {
		glm::vec2 pos;
		glm::vec4 col;
		float tex;
		glm::vec2 UVs;
	};
}

class Pipeline {

public:
	static void init();

	static void draw(size_t vertexCount, size_t indexCount);

	static void endFrame();

	static int getTextureSlots() { return textureSlots; }
	static int getVertexCount()  { return vertexCount;  }
	static int getdrawCalls()    { return drawCalls;    }

private:
	static int textureSlots;
	static int  vertexCount;
	static int    drawCalls;
};

class Renderer2D {

public:
	void init(Camera* camera, const Shader& shader);

	void pushQuad(const glm::mat4& model, const glm::vec4& color, uint32_t texID = 1);
	void flush();

	void drawBatch();

private:
	uint32_t m_Vbo, m_Ibo, m_Vao;

	data::Vertex  m_Vertices[4 * MAX_BATCH_QUAD_COUNT];
	uint32_t m_Indices[6 * MAX_BATCH_QUAD_COUNT];

	int m_TextureIndices[32 + END]; //TODO - make dependent on texSlots

	int m_BatchIndex = 0, m_TexIndex = -1;

	Shader  m_Shader;
	Camera* m_Camera;
};

