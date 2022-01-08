#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#define ARR_SZ(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

class Application {

public:
	void init(std::string windowName, int width, int height);

	GLFWwindow* getWindow() { return mWindow; }

	void run();

private:
	GLFWwindow* mWindow;
	std::string mWindowName;

	float mWidth, mHeight;
};

