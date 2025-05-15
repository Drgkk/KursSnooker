#include "ShaderProgram.h"
#include "Shaders.h"

ShaderProgram::ShaderProgram(std::unique_ptr<Shaders> shaders)
{
	this->shaders = std::move(shaders);
	ConfigureProgram();
	CheckConfigurationSuccess();
	DeleteShaders();
}

void ShaderProgram::Use()
{

}

void ShaderProgram::ConfigureProgram()
{
	SetShaderProgramID(glCreateProgram());

	shaders->AttachProgram(GetShaderProgramID());

	glLinkProgram(GetShaderProgramID());
	
}

void ShaderProgram::CheckConfigurationSuccess()
{
	int success;
	char infoLog[512];
	glGetProgramiv(GetShaderProgramID(), GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(GetShaderProgramID(), 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
	}
}

void ShaderProgram::DeleteShaders()
{
	shaders->DeleteShaders();
}

unsigned int ShaderProgram::GetShaderProgramID()
{
	return ID;
}

void ShaderProgram::SetShaderProgramID(unsigned int id)
{
	this->ID = id;
}
