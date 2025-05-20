#include "GameEventWindowResized.h"

GameEventWindowResized::GameEventWindowResized(GLFWwindow* window, int width, int height)
{
	this->window = window;
	this->width = width;
	this->height = height;
}

void GameEventWindowResized::Register()
{
	auto func = [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		};
	glfwSetFramebufferSizeCallback(window, func);
}

