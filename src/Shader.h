#pragma once

#include <cstdint>
#include <unordered_map>

#include "glm/glm.hpp"

#include "GLtype.h"

class Shader {

public:
	Shader(const std::string vertexSrc, const std::string& fragmentSrc);

	void setShader(GLtype shaderType, const std::string& shaderSrc);

	void bind() const;
	void unbind() const;

	uint32_t getRendererID() const { return mRendererID; }

	int getUniformLocation(const std::string& name);

	template<typename... T>
	void setUniform(const std::string& name, const T&... values) {
		static_assert(false, "unsupported uniform type");
	}

	template<> void setUniform(const std::string& name, const glm::vec4& f4);
	template<> void setUniform(const std::string& name, const glm::vec3& f3);
	template<> void setUniform(const std::string& name, const glm::vec2& f2);
	template<> void setUniform(const std::string& name, const float& f1);
	template<> void setUniform(const std::string& name, const glm::ivec4& i4);
	template<> void setUniform(const std::string& name, const glm::ivec3& i3);
	template<> void setUniform(const std::string& name, const glm::ivec2& i2);
	template<> void setUniform(const std::string& name, const int& i1);
	template<> void setUniform(const std::string& name, const glm::mat4& mat4);
	template<> void setUniform(const std::string& name, const glm::mat3& mat4);
	template<> void setUniform(const std::string& name, const glm::mat2& mat4);

	template<typename T, int SZ = 1> void setArrayUniform(const std::string& name, const T* arrays, uint32_t count = 1) {
		static_assert(false, "unsupported array parameters");
	}

	template<> void setArrayUniform<float, 1>(const std::string& name, const float* floats, uint32_t count);
	template<> void setArrayUniform<float, 2>(const std::string& name, const float* floats, uint32_t count);
	template<> void setArrayUniform<float, 3>(const std::string& name, const float* floats, uint32_t count);
	template<> void setArrayUniform<float, 4>(const std::string& name, const float* floats, uint32_t count);
	template<> void setArrayUniform<int, 1>(const std::string& name, const int* ints, uint32_t count);
	template<> void setArrayUniform<int, 2>(const std::string& name, const int* ints, uint32_t count);
	template<> void setArrayUniform<int, 3>(const std::string& name, const int* ints, uint32_t count);
	template<> void setArrayUniform<int, 4>(const std::string& name, const int* ints, uint32_t count);

	template<typename MT, typename T> void setMatrixArrayUniform(const std::string& name, const T* mats, uint32_t count = 1) {
		static_assert(false, "unsupported matrix parameters");
	}

	template<> void setMatrixArrayUniform<glm::mat4x4>(const std::string& name, const float* mats, uint32_t count);
	template<> void setMatrixArrayUniform<glm::mat3x3>(const std::string& name, const float* mats, uint32_t count);
	template<> void setMatrixArrayUniform<glm::mat2x2>(const std::string& name, const float* mats, uint32_t count);

	static uint32_t compileShader(GLtype shaderType, const std::string& shaderSrc);

private:
	uint32_t mRendererID = -1;
};

