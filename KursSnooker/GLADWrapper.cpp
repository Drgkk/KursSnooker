#include "GLADWrapper.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void GLADWrapper::Initialize()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw __ExceptionPtrAssign;
	}

}
