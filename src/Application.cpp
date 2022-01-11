#include "Application.h"

#include <fstream>

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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

    Pipeline::init();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Shader shader(readFile("res/shaders/VertexShader.glsl"), readFile("res/shaders/FragmentShader.glsl"));
    shader.bind();

    Camera camera;
    camera.updateViewProjection();

    Renderer2D renderer;
    renderer.init(&camera, shader);

    camera.setProjection(16, 9);
    camera.setView({ 0, 0 });
    
    camera.updateViewProjection();

    uint32_t tex1 = texture::create("res/textures/doggo.png");
    uint32_t tex2 = texture::create("res/textures/spitoon.png");


    glfwSetTime(0.0);

    double curTime = 1, prevTime = 1;

    Transform trans1;
    Transform trans2;
    Transform trans3;

    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    while (!glfwWindowShouldClose(mWindow)) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DragFloat2("transform1", glm::value_ptr(trans1.position()), 0.1f, -10.0f, 10.0f);
        ImGui::DragFloat2("transform2", glm::value_ptr(trans2.position()), 0.1f, -10.0f, 10.0f);
        ImGui::DragFloat2("transform3", glm::value_ptr(trans3.position()), 0.1f, -10.0f, 10.0f);
        ImGui::ColorPicker4("color", glm::value_ptr(color));

        curTime = glfwGetTime();

        renderer.pushQuad(trans1.modelMatrix(), color);
        renderer.pushQuad(trans2.modelMatrix(), color, tex1);
        renderer.pushQuad(trans3.modelMatrix(), color, tex2);

        Pipeline::endFrame();


        renderer.drawBatch();
        renderer.flush();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(mWindow);

        /* Poll for and process events */
        glfwPollEvents();

#if DEBUG_GL
        printf("fps: %f\n", 1.0 / (curTime - prevTime));
        printf("draw calls: %d\n", Pipeline::getdrawCalls());
#endif

        prevTime = curTime;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(mWindow);
    glfwTerminate();
}