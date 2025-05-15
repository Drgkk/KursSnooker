#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Shader
{
public:
	Shader(const char* shaderSource);
	void Compile();
	unsigned int GetShader();
	void Delete();
protected:
	virtual void CreateShader() = 0;
	virtual void ThrowErrorMessage(char info[]) = 0;
	unsigned int shader;
	const char* shaderSource;
private:
	void RegisterShader();
	void CheckRegisterShaderSuccess();
};

