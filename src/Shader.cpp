#include "Shader.h"

#include <GL/glew.h>

Shader::Shader(const std::string vertexSrc, const std::string& fragmentSrc) {
    mRendererID = glCreateProgram();

    setShader(GLtype::VERTEX_SH, vertexSrc);
    setShader(GLtype::FRAGMENT_SH, fragmentSrc);
}

void Shader::setShader(GLtype shaderType, const std::string& shaderSrc) {
    uint32_t shader = Shader::compileShader(shaderType, shaderSrc);

    glAttachShader(mRendererID, shader);
    glLinkProgram(mRendererID);

    glValidateProgram(mRendererID);

    glDeleteShader(shader);
}

void Shader::bind() const {
    glUseProgram(mRendererID);
}

void Shader::unbind() const {
    glUseProgram(0);
}

/////////////////////////////
/////Uniforms Templating/////
/////////////////////////////

int Shader::getUniformLocation(const std::string& name) {
    if (mUniformMap.find(name) == mUniformMap.end()) {
        int loc = glGetUniformLocation(mRendererID, name.c_str());
        mUniformMap[name] = loc;

        return loc;
    }

    return mUniformMap[name];
}

template<> void Shader::setUniform(const std::string& name, float f1, float f2, float f3, float f4) {
    glUniform4f(getUniformLocation(name), f1, f2, f3, f4);
}

template<> void Shader::setUniform(const std::string& name, float f1, float f2, float f3) {
    glUniform3f(getUniformLocation(name), f1, f2, f3);
}

template<> void Shader::setUniform(const std::string& name, float f1, float f2) {
    glUniform2f(getUniformLocation(name), f1, f2);
}

template<> void Shader::setUniform(const std::string& name, float f1) {
    glUniform1f(getUniformLocation(name), f1);
}

uint32_t Shader::compileShader(GLtype shaderType, const std::string& shaderSrc) {
	uint32_t id = glCreateShader((uint32_t)shaderType);

    const char* csrc = shaderSrc.c_str();

    glShaderSource(id, 1, &csrc, 0);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (!result) {
        GLint len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

        char* mes = (char*)malloc(len);
        glGetShaderInfoLog(id, len, &len, mes);
        printf("Failed to compile shader(%d)-> %s", (uint32_t)shaderType, mes);

        glDeleteShader(id);
        return 0;
    }

    return id;
}