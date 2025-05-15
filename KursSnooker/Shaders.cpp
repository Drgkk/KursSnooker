#include "Shaders.h"

void Shaders::AddShader(Shader* shader)
{
	this->shaders.push_back(shader);
}

void Shaders::AttachProgram(unsigned int shaderProgram)
{
	for (int i = 0; i < shaders.size(); i++) {
		glAttachShader(shaderProgram, shaders[i]->GetShader());
	}
}

void Shaders::DeleteShaders()
{
	for (int i = 0; i < shaders.size(); i++) {
		shaders[i]->Delete();
	}
}
