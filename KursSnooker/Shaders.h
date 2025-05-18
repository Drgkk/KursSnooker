#pragma once
#include <vector>
#include "Shader.h"
#include "ShaderProgram.h"
class Shaders
{
public:
	void AddShader(std::unique_ptr<Shader> shader);
	void AttachProgram(unsigned int shaderProgram);
	void DeleteShaders();
private:
	std::vector<std::unique_ptr<Shader>> shaders;
};

