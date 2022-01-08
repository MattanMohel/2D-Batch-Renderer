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
#include "Application.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


int main() {

    Application app;
    app.init("OpenGL", 960, 540);
    app.run();
}