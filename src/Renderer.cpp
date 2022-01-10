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

    m_Vao = vertexArray::create();
    vertexArray::bind(m_Vao);

    m_Vbo = vertexBuffer::create();
    vertexBuffer::bind(m_Vbo);
    vertexBuffer::setBuffer(nullptr, 4 * MAX_BATCH_QUAD_COUNT, sizeof(data::Vertex), gl::type::DYNAMIC_DRAW);

    m_Ibo = indexBuffer::create();
    indexBuffer::bind(m_Ibo);

    for (uint32_t i = 0; i < MAX_BATCH_QUAD_COUNT; ++i) {
        // Index Buffer
        m_Indices[i * 6 + 0] = i * 4 + 0;
        m_Indices[i * 6 + 1] = i * 4 + 1;
        m_Indices[i * 6 + 2] = i * 4 + 2;
        m_Indices[i * 6 + 3] = i * 4 + 2;
        m_Indices[i * 6 + 4] = i * 4 + 3;
        m_Indices[i * 6 + 5] = i * 4 + 0;
    }

    indexBuffer::setBuffer(m_Indices, 6 * MAX_BATCH_QUAD_COUNT, sizeof(uint32_t), gl::type::DYNAMIC_DRAW);

    size_t offset = attributes::create(0, 2, sizeof(data::Vertex), 0);
    offset = attributes::create(1, 4, sizeof(data::Vertex), offset);
    offset = attributes::create(2, 1, sizeof(data::Vertex), offset);
    offset = attributes::create(3, 2, sizeof(data::Vertex), offset);

    vertexArray::unbind();
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

    m_Vertices[m_BatchIndex * 4 + 0] = { model * glm::vec4(data::vertices[0], 1.0f, 1.0f), color, m_TexIndex - 1, data::texCoords[0] };
    m_Vertices[m_BatchIndex * 4 + 1] = { model * glm::vec4(data::vertices[1], 1.0f, 1.0f), color, m_TexIndex - 1, data::texCoords[1] };
    m_Vertices[m_BatchIndex * 4 + 2] = { model * glm::vec4(data::vertices[2], 1.0f, 1.0f), color, m_TexIndex - 1, data::texCoords[2] };
    m_Vertices[m_BatchIndex * 4 + 3] = { model * glm::vec4(data::vertices[3], 1.0f, 1.0f), color, m_TexIndex - 1, data::texCoords[3] };

    ++m_BatchIndex;
}

void Renderer2D::flush() {
    m_BatchIndex = 0;
    m_TexIndex   = 0;
}

void Renderer2D::drawBatch() {
    vertexArray::bind(m_Vao);

    m_Shader.setUniform("u_VP", m_Camera->getViewProjection());
    m_Shader.setArrayUniform("u_Textures", m_TextureUniform, m_TexIndex);

    glNamedBufferSubData(m_Vbo, 0, sizeof(m_Vertices), m_Vertices);

    Pipeline::draw(4 * m_BatchIndex, 6 * m_BatchIndex);
}
