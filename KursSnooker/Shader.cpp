#include "Shader.h"

Shader::Shader(const char* shaderSource) {
	std::string str;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		shaderFile.open(shaderSource);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		str = shaderStream.str();
	}
	catch (std::ifstream::failure& e){
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}
	this->shaderCodeStr = str;
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
	const char* shaderCode = shaderCodeStr.c_str();
	glShaderSource(shader, 1, &shaderCode, NULL);
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