#pragma once

#include <cstdint>
#include <unordered_map>

#include "GLtype.h"

class Shader {

public:
	Shader(const std::string vertexSrc, const std::string& fragmentSrc);

	void setShader(GLtype shaderType, const std::string& shaderSrc);

	void bind() const;
	void unbind() const;

	int getUniformLocation(const std::string& name);

	template<typename... T>
	void setUniform(const std::string& name, T... values) {
		static_assert(false, "unsupported uniform type");
	}

	template<> void setUniform(const std::string& name, float f1, float f2, float f3, float f4);
	template<> void setUniform(const std::string& name, float f1, float f2, float f3);
	template<> void setUniform(const std::string& name, float f1, float f2);
	template<> void setUniform(const std::string& name, float f1);

	static uint32_t compileShader(GLtype shaderType, const std::string& shaderSrc);

private:
	uint32_t mRendererID = -1;

	std::unordered_map<std::string, int> mUniformMap;
};

