#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
public:
	Window(int width, int height, const char* title);
	int GetWidth();
	int GetHeight();
	GLFWwindow* GetGLFWWindow();
private:
	void GenerateWindow(int width, int height, const char* title);
	GLFWwindow* window;
	int width;
	int height;
	const char* title;
};

