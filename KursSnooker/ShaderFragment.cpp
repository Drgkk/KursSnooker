#include "ShaderFragment.h"

void ShaderFragment::CreateShader() {
	shader = glCreateShader(GL_FRAGMENT_SHADER);
}

void ShaderFragment::ThrowErrorMessage(char info[]) {
	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info << "\n";
	throw __ExceptionPtrCopy;
}