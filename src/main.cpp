#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <fstream>
#include <csignal>
#include <string>

#include "Buffer.h"
#include "Attribute.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"

#define ARR_SZ(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

static std::string readFile(const std::string& path) {
    std::ifstream stream(path);

    std::string source, line;
    while (getline(stream, line)) {
        source += line;
        source += "\n";
    }

    return source;
}

static void glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        printf("notification: %s\n", message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        printf("low-severity %s\n", message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        printf("medium-severity: %s\n", message);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        printf("high-severity: %s\n", message);
        __debugbreak();
    }
}

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Enable callback debugging */
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "OpenGL-Testing", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        printf("Error\n"); 
    }

    printf("%s\n", glGetString(GL_VERSION));
    printf("have %d texture slots", Texture::textureSlotCount());

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //glDebugMessageCallback(glDebugCallback, nullptr);

    /* Vertex buffer */
    float vertices[] = {
        /*Pos*/-1.0f, -1.0f,/*UV*/0.0f, 0.0f,
        /*Pos*/ 1.0f, -1.0f,/*UV*/1.0f, 0.0f,
        /*Pos*/ 1.0f,  1.0f,/*UV*/1.0f, 1.0f, 
        /*Pos*/-1.0f,  1.0f,/*UV*/0.0f, 1.0f,
    };

    /* Index buffer */
    GLuint indices[] = {
        0, 1, 2, 
        2, 3, 0
    };

    VertexArray vao;
    vao.setIndexBuffer(IndexBuffer(indices, 6));
    vao.setVertexBuffer(VertexBuffer(vertices, 16), BufferLayout({ {"pos", GLtype::FLOAT, 2, false}, {"uv", GLtype::FLOAT, 2, false} }));
    vao.bind();

    Shader shader(readFile("res/shaders/VertexShader.shader"), readFile("res/shaders/FragmentShader.shader"));
    shader.bind();

    Texture tex("res/textures/doggo.png");
    tex.bind();

    shader.setUniform("u_Texture", 0);

    int loc = shader.getUniformLocation("u_Color");
    float r = 0.0f, incr = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shader.setUniform("u_Color", r, 0.3f, 0.0f, 1.0f);

        if (r > 1 || r < 0) {
            incr *= -1;
        }
        r += incr;

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}