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

int Pipeline::textureSlots = 0;
int Pipeline::vertexCount  = 0;
int Pipeline::drawCalls    = 0;

void Pipeline::init() {
    if (glewInit() != GLEW_OK) {
        //ASSERT
    }

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureSlots);

#if DEBUG_GL
    printf("%s\n", glGetString(GL_VERSION));

    glDebugMessageCallback(glDebugCallback, nullptr);
#endif

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void Pipeline::draw(size_t vertexCount, size_t indexCount) {
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    ++drawCalls;
    vertexCount += vertexCount;
}

void Pipeline::endFrame() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawCalls   = 0;
    vertexCount = 0;
}

void Renderer2D::init(Camera* camera, const Shader& shader) {
    m_Camera = camera;
    m_Shader = shader;

    m_Vao = VertexArray::create();
    VertexArray::bind(m_Vao);
    m_Vbo = VertexBuffer::create(nullptr, 4 * MAX_BATCH_QUAD_COUNT, sizeof(Vertex), GLtype::DYNAMIC_DRAW);
    VertexBuffer::bind(m_Vbo);
    m_Ibo = IndexBuffer::create(nullptr, 6 * MAX_BATCH_QUAD_COUNT, GLtype::DYNAMIC_DRAW);
    IndexBuffer::bind(m_Ibo);

    for (uint32_t i = 0; i < MAX_BATCH_QUAD_COUNT; ++i) {
        // Index Buffer
        m_Indices[i * 6 + 0] = i * 4 + 0;
        m_Indices[i * 6 + 1] = i * 4 + 1;
        m_Indices[i * 6 + 2] = i * 4 + 2;
        m_Indices[i * 6 + 3] = i * 4 + 2;
        m_Indices[i * 6 + 4] = i * 4 + 3;
        m_Indices[i * 6 + 5] = i * 4 + 0;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices), m_Indices, GL_DYNAMIC_DRAW);

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

void Renderer2D::pushQuad(const glm::mat4& model, const glm::vec4& color, uint32_t texID) {
    if (m_BatchIndex == MAX_BATCH_QUAD_COUNT || m_TexIndex >= Pipeline::getTextureSlots()) {
        drawBatch();
        flush();
    }

    if (texID > m_TexIndex) {
        glBindTextureUnit(m_TexIndex + 1, texID);
        m_TextureUniform[m_TexIndex] = texID;

        ++m_TexIndex;
    }

    m_Vertices[m_BatchIndex * 4 + 0] = { model * glm::vec4(vertices[0], 1.0f, 1.0f), color, m_TexIndex - 1, texCoords[0] };
    m_Vertices[m_BatchIndex * 4 + 1] = { model * glm::vec4(vertices[1], 1.0f, 1.0f), color, m_TexIndex - 1, texCoords[1] };
    m_Vertices[m_BatchIndex * 4 + 2] = { model * glm::vec4(vertices[2], 1.0f, 1.0f), color, m_TexIndex - 1, texCoords[2] };
    m_Vertices[m_BatchIndex * 4 + 3] = { model * glm::vec4(vertices[3], 1.0f, 1.0f), color, m_TexIndex - 1, texCoords[3] };

    ++m_BatchIndex;
}

void Renderer2D::flush() {
    m_BatchIndex = 0;
    m_TexIndex   = 0;
}

void Renderer2D::drawBatch() {
    VertexArray::bind(m_Vao);

    m_Shader.setUniform("u_VP", m_Camera->getViewProjection());
    m_Shader.setArrayUniform("u_Textures", m_TextureUniform, m_TexIndex);

    glNamedBufferSubData(m_Vbo, 0, sizeof(m_Vertices), m_Vertices);

    Pipeline::draw(4 * m_BatchIndex, 6 * m_BatchIndex);
}
