#pragma once
#include <memory>

class Shaders;

class ShaderProgram
{
public:
	ShaderProgram(Shaders* shaders);
	void Use();
private:
	void ConfigureProgram();
	void CheckConfigurationSuccess();
	void DeleteShaders();
	unsigned int GetShaderProgramID();
	void SetShaderProgramID(unsigned int id);
	unsigned int ID;
	Shaders* shaders;
};

