#include "Application.h"

#include <fstream>

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Transform.h"

#include "Renderer.h"

static std::string readFile(const std::string& path) {
    std::ifstream stream(path);

    std::string source, line;
    while (getline(stream, line)) {
        source += line;
        source += "\n";
    }

    return source;
}

void Application::initGLFW() {
    if (!glfwInit()) {
        //ASSERT
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if DEBUG_GL
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
}

void Application::initWindow(std::string windowName, int width, int height) {
    mWindow = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);

    if (!mWindow) {
        glfwTerminate();
        //ASSERT
    }

    glfwMakeContextCurrent(mWindow);

    glfwSwapInterval(1);
}

void Application::run() {

#if 0
    Renderer renderer;
    renderer.init(false);

    /* Vertex buffer */
    float vertices[] = {
        /*Pos*/-1.0f, -1.0f, /*RGBA*/ 1.0f, 1.0f, 1.0f, 1.0f, /*texture*/ 0.0f, /*UV*/ 0.0f, 0.0f, /*mvp*/ 0.0f,
        /*Pos*/-1.0f,  1.0f, /*RGBA*/ 1.0f, 1.0f, 1.0f, 1.0f, /*texture*/ 0.0f, /*UV*/ 0.0f, 1.0f, /*mvp*/ 0.0f,
        /*Pos*/ 1.0f,  1.0f, /*RGBA*/ 1.0f, 1.0f, 1.0f, 1.0f, /*texture*/ 0.0f, /*UV*/ 1.0f, 1.0f, /*mvp*/ 0.0f,
        /*Pos*/ 1.0f, -1.0f, /*RGBA*/ 1.0f, 1.0f, 1.0f, 1.0f, /*texture*/ 0.0f, /*UV*/ 1.0f, 0.0f, /*mvp*/ 0.0f,
    };

    /* Index buffer */
    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,
    };

    Shader shader(readFile("res/shaders/VertexShader.glsl"), readFile("res/shaders/FragmentShader.glsl"));
    shader.bind();

    VertexArray vao;

    vao.setIndexBuffer(IndexBuffer(indices, ARR_SZ(indices)));
    vao.setVertexBuffer(VertexBuffer(vertices, ARR_SZ(vertices)), BufferLayout({
        {GLtype::FLOAT, 2, false }, /*positions*/
        {GLtype::FLOAT, 4, false }, /*rgba*/
        {GLtype::FLOAT, 1, false }, /*texture slot*/
        {GLtype::FLOAT, 2, false }, /*uv coord*/
        {GLtype::FLOAT, 1, false }, /*mvp slot*/ }));
    vao.bind();

    /* Camera Projection */
    glm::mat4 proj = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // 'translate' to the right

    glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0)); // model 'transform'
    glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, 0)); // model 'transform'

    glm::mat4 mvp1 = proj * view * model1;
    glm::mat4 mvp2 = proj * view * model2;

    float f[32];
    memcpy(&f[0], glm::value_ptr(mvp1), 16 * sizeof(float));
    memcpy(&f[16], glm::value_ptr(mvp2), 16 * sizeof(float));

    shader.setMatrixArrayUniform<glm::mat4>("u_MVPs", f, 2);

    glBindTextureUnit(1, Texture::createTexture("res/textures/doggo.png"));
    glBindTextureUnit(2, Texture::createTexture("res/textures/spitoon.png"));

    int textureSlots[] = { 1, 2 };
    shader.setArrayUniform("u_Textures", textureSlots, 2);
#endif

#if 1
    Renderer::initGLEW();

    Renderer renderer;
    renderer.initBatching();

    Shader shader(readFile("res/shaders/VertexShader.glsl"), readFile("res/shaders/FragmentShader.glsl"));
    shader.bind();

    ///* Camera Projection */
    glm::mat4 proj = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // 'translate' to the right
    
    glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0)); // model 'transform'
    glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, 0)); // model 'transform'
    
    glm::mat4 mvp1 = proj * view * model1;
    glm::mat4 mvp2 = proj * view * model2;

    uint32_t tex1 = Texture::createTexture("res/textures/doggo.png");
    uint32_t tex2 = Texture::createTexture("res/textures/spitoon.png");

#endif

    float r = 0.0f;
    float incr = 0.01f;

    glfwSetTime(0.0);

    double curTime = 1, prevTime = 1;

    renderer.mShader = shader;
    renderer.m_ViewProjection = proj * view;

    // pos * proj * view * model

    Transform trans;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(mWindow)) {
        /* Render here */

        curTime = glfwGetTime();

        for (int i = -16; i < 16; ++i) {
            for (int j = -9; j < 9; ++j) {
                trans.position() = {(float)i, (float)j};
                renderer.pushQuad(trans.modelMatrix(), glm::vec4(r, 1.0f, 1.0f, 1.0f), tex1);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        if (r >= 1 || r <= 0) {
            r = r >= 1? 1.0f : 0.0f;
            incr *= -1;
        }

        r += incr;


        renderer.drawBatch();
        renderer.flush();

        /* Swap front and back buffers */
        glfwSwapBuffers(mWindow);

        /* Poll for and process events */
        glfwPollEvents();

        printf("fps: %f\n", 1.0 / (curTime - prevTime));

        prevTime = curTime;

        printf("draw calls: %d\n", renderer.queryFlushCount());
#if DEBUG_GL
#endif
    }

    glfwTerminate();
}