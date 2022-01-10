#include "Renderer.h"

#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

static void glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        printf("notification(%d): %s\n", id, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        printf("low-severity(%d): %s\n", id, message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        printf("medium-severity(%d): %s\n", id, message);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        printf("high-severity(%d): %s\n", id, message);
        __debugbreak();
    }
}

uint32_t Renderer::maxTextureSlots = 0;

void Renderer::initGLEW() {
    if (glewInit() != GLEW_OK) {
        //ASSERT
    }

    maxTextureSlots = Texture::textureSlotCount();

#if DEBUG_GL
    printf("%s\n", glGetString(GL_VERSION));

    glDebugMessageCallback(glDebugCallback, nullptr);
#endif

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void Renderer::initBatching() {

    mVertexArrayID = VertexArray::create();
    VertexArray::bind(mVertexArrayID);

    mVertexBufferID = VertexBuffer::create(nullptr, 4 * MAX_BATCH_QUAD_COUNT, sizeof(Vertex), GLtype::DYNAMIC_DRAW);
    VertexBuffer::bind(mVertexBufferID);

    mIndexBufferID = IndexBuffer::create(nullptr, 6 * MAX_BATCH_QUAD_COUNT, GLtype::DYNAMIC_DRAW);
    IndexBuffer::bind(mIndexBufferID);

    for (uint32_t i = 0; i < MAX_BATCH_QUAD_COUNT; ++i) {
        // Index Buffer
        mBatchIndexBuffer[i * 6 + 0] = i * 4 + 0;
        mBatchIndexBuffer[i * 6 + 1] = i * 4 + 1;
        mBatchIndexBuffer[i * 6 + 2] = i * 4 + 2;
        mBatchIndexBuffer[i * 6 + 3] = i * 4 + 2;
        mBatchIndexBuffer[i * 6 + 4] = i * 4 + 3;
        mBatchIndexBuffer[i * 6 + 5] = i * 4 + 0;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mBatchIndexBuffer), mBatchIndexBuffer, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, col));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, tex));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, UVs));

    VertexArray::unbind();
}

void Renderer::pushQuad(const glm::mat4& model, const glm::vec4& color, uint32_t texID) {
    if (mBufferIndex == MAX_BATCH_QUAD_COUNT || mTextureIndex >= maxTextureSlots) {
        drawBatch();
        flush();
    }

    if (texID > mTextureIndex) {
        glBindTextureUnit(mTextureIndex + 1, texID);
        mTextureArray[mTextureIndex] = texID;
        ++mTextureIndex;
    }
    
    mBatchVertexBuffer[mBufferIndex * 4 + 0] = { model * glm::vec4(vertices[0], 1.0f, 1.0f), color, mTextureIndex - 1, texCoords[0] };
    mBatchVertexBuffer[mBufferIndex * 4 + 1] = { model * glm::vec4(vertices[1], 1.0f, 1.0f), color, mTextureIndex - 1, texCoords[1] };
    mBatchVertexBuffer[mBufferIndex * 4 + 2] = { model * glm::vec4(vertices[2], 1.0f, 1.0f), color, mTextureIndex - 1, texCoords[2] };
    mBatchVertexBuffer[mBufferIndex * 4 + 3] = { model * glm::vec4(vertices[3], 1.0f, 1.0f), color, mTextureIndex - 1, texCoords[3] };

    ++mBufferIndex;
}

void Renderer::flush() {
    ++mFlushCount;
    mTextureIndex = 0;
    mBufferIndex = 0;
}

uint32_t Renderer::queryFlushCount() { 
    uint32_t flushCount = mFlushCount; 
    mFlushCount = 0; return flushCount; 
}

void Renderer::drawBatch() {
    VertexArray::bind(mVertexArrayID);

    mShader.bind();

    mShader.setUniform("u_VP", m_ViewProjection);
    mShader.setArrayUniform("u_Textures", mTextureArray, mTextureIndex);

    glNamedBufferSubData(mVertexBufferID, 0, sizeof(mBatchVertexBuffer), mBatchVertexBuffer);

    glDrawElements(GL_TRIANGLES, 6 * mBufferIndex, GL_UNSIGNED_INT, nullptr);
}

void Renderer::draw(const VertexArray& vertexArray, const Shader& shader) {
    vertexArray.bind();
    shader.bind();

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}