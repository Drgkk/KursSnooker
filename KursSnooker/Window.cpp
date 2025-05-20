#include "Window.h"

Window::Window(int width, int height, const char* title)
{
	glfwInit();
	GenerateWindow(width, height, title);
	glfwMakeContextCurrent(window);
	//glViewport(0, 0, 800, 600);
}

int Window::GetWidth()
{
	return this->width;
}

int Window::GetHeight()
{
	return this->height;
}

void Window::SetWidth(int width)
{
	this->width = width;
}

void Window::SetHeight(int height)
{
	this->height = height;
}

GLFWwindow* Window::GetGLFWWindow()
{
	return this->window;
}

void Window::GenerateWindow(int width, int height, const char* title)
{
	this->width = width;
	this->height = height;
	this->title = title;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, "Snooker", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to generate GLFW window" << std::endl;
		glfwTerminate();
		throw __ExceptionPtrAssign;
	}
}
