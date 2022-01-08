#include "Renderer.h"

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

void Renderer::init(bool initBatchBuffer) {
    if (glewInit() != GLEW_OK) {
        //ASSERT
    }

#if DEBUG_GL
    printf("%s\n", glGetString(GL_VERSION));
    printf("%d texture slots\n", Texture::textureSlotCount());

    glDebugMessageCallback(glDebugCallback, nullptr);
#endif

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    if (!initBatchBuffer) {
        return;
    }

    mVertexArrayID = VertexArray::createVertexArray();
    VertexArray::bind(mVertexArrayID);

    mVertexBufferID = VertexBuffer::createVertexBuffer(nullptr, 4 * MAX_BATCH_QUAD_COUNT, sizeof(Vertex), GLtype::DYNAMIC_DRAW);
    VertexBuffer::bind(mVertexBufferID);

    mIndexBufferID = IndexBuffer::createIndexBuffer(nullptr, 6 * MAX_BATCH_QUAD_COUNT, GLtype::DYNAMIC_DRAW);
    IndexBuffer::bind(mIndexBufferID);

    for (int i = 0; i < (MAX_BATCH_QUAD_COUNT); ++i) {
        mBatchIndexBuffer[i * 6 + 0] = i * 4 + 0;
        mBatchIndexBuffer[i * 6 + 1] = i * 4 + 1;
        mBatchIndexBuffer[i * 6 + 2] = i * 4 + 2;
        mBatchIndexBuffer[i * 6 + 3] = i * 4 + 2;
        mBatchIndexBuffer[i * 6 + 4] = i * 4 + 3;
        mBatchIndexBuffer[i * 6 + 5] = i * 4 + 0;
    }
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mBatchIndexBuffer), mBatchIndexBuffer, GL_DYNAMIC_DRAW);

    // init vertex buffer
    for (int i = 0; i < (MAX_BATCH_QUAD_COUNT); ++i) {
        /*pos, rgba, tex, uvs, mvp*/
        mBatchVertexBuffer[i * 4 + 0] = { glm::vec2(-1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), (float)i, glm::vec2(0.0f, 0.0f), (float)i };
        mBatchVertexBuffer[i * 4 + 1] = { glm::vec2(-1.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), (float)i, glm::vec2(0.0f, 1.0f), (float)i };
        mBatchVertexBuffer[i * 4 + 2] = { glm::vec2( 1.0f,  1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), (float)i, glm::vec2(1.0f, 1.0f), (float)i };
        mBatchVertexBuffer[i * 4 + 3] = { glm::vec2( 1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), (float)i, glm::vec2(1.0f, 0.0f), (float)i };
    }

    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, col));
    glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, tex));
    glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, UVs));
    glVertexAttribPointer(4, 1, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, mvp));
}

void Renderer::draw(const VertexArray& vertexArray, const Shader& shader) {
    vertexArray.bind();
    shader.bind();

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}

void Renderer::pushQuad(const glm::mat4& mvp, const glm::vec4& color, uint32_t texID) {
    memcpy(&mMatrixArray[mBufferIndex * 16], glm::value_ptr(mvp), 16 * sizeof(float));
    mTextureArray[mBufferIndex] = texID;
    
    mBatchVertexBuffer[mBufferIndex * 4 + 0].col = color;
    mBatchVertexBuffer[mBufferIndex * 4 + 1].col = color;
    mBatchVertexBuffer[mBufferIndex * 4 + 2].col = color;
    mBatchVertexBuffer[mBufferIndex * 4 + 3].col = color;    
    
    mBatchVertexBuffer[mBufferIndex * 4 + 0].tex = mBufferIndex;
    mBatchVertexBuffer[mBufferIndex * 4 + 1].tex = mBufferIndex;
    mBatchVertexBuffer[mBufferIndex * 4 + 2].tex = mBufferIndex;
    mBatchVertexBuffer[mBufferIndex * 4 + 3].tex = mBufferIndex;

    glBindTextureUnit(mBufferIndex + 1, texID);

    ++mBufferIndex;
}

void Renderer::flush() {
    mBufferIndex = 0;
}

void Renderer::drawBatch(const Shader& shader) {
    VertexArray::bind(mVertexArrayID);
    shader.bind();

    shader.setMatrixArrayUniform<glm::mat4x4>("u_MVPs", mMatrixArray, mBufferIndex);
    shader.setArrayUniform("u_Textures", mTextureArray, mBufferIndex);

    glNamedBufferSubData(mVertexBufferID, 0, sizeof(mBatchVertexBuffer), mBatchVertexBuffer);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6 * mBufferIndex, GL_UNSIGNED_INT, nullptr);
}