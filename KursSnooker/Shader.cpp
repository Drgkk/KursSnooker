#include "Shader.h"

Shader::Shader(const char* shaderSource) {
	this->shaderSource = shaderSource;
}

void Shader::Compile()
{
	CreateShader();
	RegisterShader();
	CheckRegisterShaderSuccess();
}

unsigned int Shader::GetShader()
{
	return shader;
}

void Shader::Delete()
{
	glDeleteShader(shader);
}

void Shader::RegisterShader() {
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	
}

void Shader::CheckRegisterShaderSuccess() {
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		ThrowErrorMessage(infoLog);
	}
}