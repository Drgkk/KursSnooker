#pragma once
#include <vector>
#include "Shader.h"
#include "ShaderProgram.h"
class Shaders
{
public:
	void AddShader(Shader* shader);
	void AttachProgram(unsigned int shaderProgram);
	void DeleteShaders();
private:
	std::vector<Shader*> shaders;
};

