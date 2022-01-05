#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <fstream>
#include <csignal>
#include <string>

#include "Buffer.h"
#include "Attribute.h"
#include "VertexArray.h"

static std::string readFile(const std::string& path) {
    std::ifstream stream(path);

    std::string source, line;
    while (getline(stream, line)) {
        source += line;
        source += "\n";
    }

    return source.c_str();
}

static GLuint compileShader(GLenum type, const std::string& src) {
    GLuint id = glCreateShader(type);
    const char* csrc = src.c_str();

    /* Creates a shader source from csrc*/
    glShaderSource(id, 1, &csrc, 0);
    glCompileShader(id);

    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        GLint len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

        char* mes = (char*)alloca(len);
        glGetShaderInfoLog(id, len, &len, mes);
        printf("Failed to compile shader(%d)-> %s", (int)type, mes);

        glDeleteShader(id);
        return 0;
    }

    return id;
} 

/* creates shader from actual source code */
static GLuint createShader(const std::string& vertPath, const std::string& fragPath) {
    GLuint id = glCreateProgram();
    GLuint vs = compileShader(GL_VERTEX_SHADER, readFile(vertPath));
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, readFile(fragPath));

    glAttachShader(id, vs);
    glAttachShader(id, fs);
    glLinkProgram(id);

    glValidateProgram(id);

    /* Delete intermidiary shader files */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return id;
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

    glDebugMessageCallback(glDebugCallback, nullptr);

    /* Vertex buffer */
    float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    /* Index buffer */
    GLuint indices[] = {
        0, 1, 2, 
        2, 3, 0
    };

    VertexArray vao;

    vao.setIndexBuffer(IndexBuffer(indices, 6));
    vao.setVertexBuffer(VertexBuffer(vertices, 8, { {/*type*/GLtype::FLOAT, /*count*/2, /*normalized*/false} }));
    vao.bind();

    //GLuint vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao); 

    //VertexBuffer vbo(vertices, 8);
    //vbo.bind();

    //BufferLayout attrib({ {/*type*/GLtype::FLOAT, /*count*/2, /*normalized*/false} });
    //attrib.bind();

    //IndexBuffer ibo(indices, 6);
    //ibo.bind();

    GLuint shader = createShader("res/shaders/VertexShader.shader", "res/shaders/FragmentShader.shader");
    glUseProgram(shader);

    int loc = glGetUniformLocation(shader, "u_Color");
    float r = 0.0f, incr = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform4f(loc, r, 0.3f, 0.0f, 1.0f);

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

    glDeleteProgram(shader);

    glfwTerminate();
}