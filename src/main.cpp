#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Application.h"

int main() {

    Application::initGLFW();

    Application app;
    app.initWindow("OpenGL", 960, 540);
    app.run();
}