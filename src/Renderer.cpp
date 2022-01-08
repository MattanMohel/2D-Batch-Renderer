#include "Renderer.h"

#include <GL/glew.h>

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

void Renderer::init() {
#if DEBUG_GL
    printf("%s\n", glGetString(GL_VERSION));
    printf("have %d texture slots\n", Texture::textureSlotCount());

    glDebugMessageCallback(glDebugCallback, nullptr);
#endif

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void Renderer::draw(const VertexArray& vertexArray, const Shader& shader) {
    vertexArray.bind();
    shader.bind();

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}