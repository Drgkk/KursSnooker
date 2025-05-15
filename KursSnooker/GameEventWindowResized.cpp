#include "GameEventWindowResized.h"

GameEventWindowResized::GameEventWindowResized(Window* window)
{
	this->window = window->GetGLFWWindow();
	this->width = window->GetWidth();
	this->height = window->GetHeight();
}

void GameEventWindowResized::Register()
{
	auto func = [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		};
	glfwSetFramebufferSizeCallback(window, func);
}

