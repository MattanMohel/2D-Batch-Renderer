#include "Shader.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

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
    return glGetUniformLocation(mRendererID, name.c_str());
}

template<> void Shader::setUniform(const std::string& name, const glm::vec4& f4) const {
    glUniform4f(glGetUniformLocation(mRendererID, name.c_str()), f4[0], f4[1], f4[2], f4[3]);
}

template<> void Shader::setUniform(const std::string& name, const glm::vec3& f3) const {
    glUniform3f(glGetUniformLocation(mRendererID, name.c_str()), f3[0], f3[1], f3[2]);
}

template<> void Shader::setUniform(const std::string& name, const glm::vec2& f2) const {
    glUniform2f(glGetUniformLocation(mRendererID, name.c_str()), f2[0], f2[1]);
}

template<> void Shader::setUniform(const std::string& name, const float& f1) const {
    glUniform1f(glGetUniformLocation(mRendererID, name.c_str()), f1);
}

template<> void Shader::setUniform(const std::string& name, const glm::ivec4& i4) const {
    glUniform4i(glGetUniformLocation(mRendererID, name.c_str()), i4[0], i4[1], i4[2], i4[3]);
}

template<> void Shader::setUniform(const std::string& name, const glm::ivec3& i3) const {
    glUniform3i(glGetUniformLocation(mRendererID, name.c_str()), i3[0], i3[1], i3[2]);
}

template<> void Shader::setUniform(const std::string& name, const glm::ivec2& i2) const {
    glUniform2i(glGetUniformLocation(mRendererID, name.c_str()), i2[0], i2[1]);
}

template<> void Shader::setUniform(const std::string& name, const int& i1) const {
    glUniform1i(glGetUniformLocation(mRendererID, name.c_str()), i1);
}

template<> void Shader::setUniform(const std::string& name, const glm::mat4& mat4) const {
    glUniformMatrix4fv(glGetUniformLocation(mRendererID, name.c_str()), 1, false, glm::value_ptr(mat4));
}

template<> void Shader::setUniform(const std::string& name, const glm::mat3& mat3) const {
    glUniformMatrix3fv(glGetUniformLocation(mRendererID, name.c_str()), 1, false, glm::value_ptr(mat3));
}

template<> void Shader::setUniform(const std::string& name, const glm::mat2& mat2) const {
    glUniformMatrix2fv(glGetUniformLocation(mRendererID, name.c_str()), 1, false, glm::value_ptr(mat2));
}

template<> void Shader::setArrayUniform<float, 1>(const std::string& name, const float* floats, uint32_t count) const {
    glUniform1fv(glGetUniformLocation(mRendererID, name.c_str()), count, floats);
}

template<> void Shader::setArrayUniform<float, 2>(const std::string& name, const float* floats, uint32_t count) const {
    glUniform2fv(glGetUniformLocation(mRendererID, name.c_str()), count, floats);
}

template<> void Shader::setArrayUniform<float, 3>(const std::string& name, const float* floats, uint32_t count) const {
    glUniform3fv(glGetUniformLocation(mRendererID, name.c_str()), count, floats);
}

template<> void Shader::setArrayUniform<float, 4>(const std::string& name, const float* floats, uint32_t count) const {
    glUniform4fv(glGetUniformLocation(mRendererID, name.c_str()), count, floats);
}

template<> void Shader::setArrayUniform<int, 1>(const std::string& name, const int* ints, uint32_t count) const {
    glUniform1iv(glGetUniformLocation(mRendererID, name.c_str()), count, ints);
}

template<> void Shader::setArrayUniform<int, 2>(const std::string& name, const int* ints, uint32_t count) const {
    glUniform2iv(glGetUniformLocation(mRendererID, name.c_str()), count, ints);
}

template<> void Shader::setArrayUniform<int, 3>(const std::string& name, const int* ints, uint32_t count) const {
    glUniform3iv(glGetUniformLocation(mRendererID, name.c_str()), count, ints);
}

template<> void Shader::setArrayUniform<int, 4>(const std::string& name, const int* ints, uint32_t count) const {
    glUniform4iv(glGetUniformLocation(mRendererID, name.c_str()), count, ints);
}

template<> void Shader::setMatrixArrayUniform<glm::mat4x4>(const std::string& name, const float* mats, uint32_t count) const {
    glUniformMatrix4fv(glGetUniformLocation(mRendererID, name.c_str()), count, false, mats);
}

template<> void Shader::setMatrixArrayUniform<glm::mat3x3>(const std::string& name, const float* mats, uint32_t count) const {
    glUniformMatrix3fv(glGetUniformLocation(mRendererID, name.c_str()), count, false, mats);
}

template<> void Shader::setMatrixArrayUniform<glm::mat2x2>(const std::string& name, const float* mats, uint32_t count) const {
    glUniformMatrix2fv(glGetUniformLocation(mRendererID, name.c_str()), count, false, mats);
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