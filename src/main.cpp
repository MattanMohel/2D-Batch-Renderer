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
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define ARR_SZ(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

template<int SZ>
void func() {
    if constexpr (SZ == 1) {
        static_assert(false, "do not 1");
    }
}

static std::string readFile(const std::string& path) {
    std::ifstream stream(path);

    std::string source, line;
    while (getline(stream, line)) {
        source += line;
        source += "\n";
    }

    return source;
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
    window = glfwCreateWindow(960, 540, "OpenGL-Testing", NULL, NULL);
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

    Renderer::init();

    /* Vertex buffer */
    float vertices[] = {
        /*Pos*/-1.5f, -0.5f, /*RGBA*/ 1.0f, 0.0f, 0.0f, 1.0f, /*texture*/ 0.0f, /*UV*/ 0.0f, 0.0f, /*mvp*/ 0.0f,
        /*Pos*/-0.5f, -0.5f, /*RGBA*/ 0.0f, 1.0f, 0.0f, 1.0f, /*texture*/ 0.0f, /*UV*/ 1.0f, 0.0f, /*mvp*/ 0.0f,
        /*Pos*/-0.5f,  0.5f, /*RGBA*/ 0.0f, 0.0f, 1.0f, 1.0f, /*texture*/ 0.0f, /*UV*/ 1.0f, 1.0f, /*mvp*/ 0.0f,
        /*Pos*/-1.5f,  0.5f, /*RGBA*/ 1.0f, 1.0f, 1.0f, 1.0f, /*texture*/ 0.0f, /*UV*/ 0.0f, 1.0f, /*mvp*/ 0.0f,     
        
        /*Pos*/-1.5f, -0.5f, /*RGBA*/ 1.0f, 0.0f, 0.0f, 1.0f, /*texture*/ 1.0f, /*UV*/ 0.0f, 0.0f, /*mvp*/ 1.0f,
        /*Pos*/-0.5f, -0.5f, /*RGBA*/ 0.0f, 1.0f, 0.0f, 1.0f, /*texture*/ 1.0f, /*UV*/ 1.0f, 0.0f, /*mvp*/ 1.0f,
        /*Pos*/-0.5f,  0.5f, /*RGBA*/ 0.0f, 0.0f, 1.0f, 1.0f, /*texture*/ 1.0f, /*UV*/ 1.0f, 1.0f, /*mvp*/ 1.0f,
        /*Pos*/-1.5f,  0.5f, /*RGBA*/ 1.0f, 1.0f, 1.0f, 1.0f, /*texture*/ 1.0f, /*UV*/ 0.0f, 1.0f, /*mvp*/ 1.0f,
    };

    /* Index buffer */ 
    uint32_t indices[] = {
        0, 1, 2, 
        2, 3, 0,

        4, 5, 6, 
        6, 7, 4,
    };

    VertexArray vao;

    vao.setIndexBuffer(IndexBuffer(indices, 12));
    vao.setVertexBuffer(VertexBuffer(vertices, 80), BufferLayout({
        {GLtype::FLOAT, 2, false }, /*positions*/
        {GLtype::FLOAT, 4, false }, /*rgba*/
        {GLtype::FLOAT, 1, false }, /*texture slot*/
        {GLtype::FLOAT, 2, false }, /*uv coord*/
        {GLtype::FLOAT, 1, false }, /*mvp slot*/ }));
    vao.bind();

    Shader shader(readFile("res/shaders/VertexShader.glsl"), readFile("res/shaders/FragmentShader.glsl"));
    shader.bind();

    //Texture tex("res/textures/doggo.png");
    //tex.bind();
    //
    //shader.setUniform("uTexture", 0);

    /* Camera Projection */
    glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // 'translate' to the right
    
    glm::mat4 model1 = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0)), 0.5f, glm::vec3(0, 0, 1)); // model 'transform'
    glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, 0)); // model 'transform'

    glm::mat4 mvp1 = proj * view * model1;
    glm::mat4 mvp2 = proj * view * model2;

    float* f = (float*)malloc(sizeof(float) * 32);
    memcpy(f, glm::value_ptr(mvp1), 16 * sizeof(float));
    memcpy(&f[16], glm::value_ptr(mvp2), 16 * sizeof(float));

    shader.setMatrixArrayUniform<glm::mat4>("u_MVPs", f, 2);

    glBindTextureUnit(1, Texture::loadTexture("res/textures/doggo.png"));
    glBindTextureUnit(2, Texture::loadTexture("res/textures/spitoon.png"));

    int textureSlots[] = { 1, 2 };
    shader.setArrayUniform("u_Textures", textureSlots, 2);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */

        Renderer::draw(vao, shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}