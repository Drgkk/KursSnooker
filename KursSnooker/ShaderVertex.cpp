#include "ShaderVertex.h"

void ShaderVertex::CreateShader() {
	shader = glCreateShader(GL_VERTEX_SHADER);
}
void ShaderVertex::ThrowErrorMessage(char info[]) {
	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info << "\n";
	throw __ExceptionPtrCopy;
}

