#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(Shaders& shaders) : shaders(shaders)
{
	ConfigureProgram();
	CheckConfigurationSuccess();
	DeleteShaders();
}

void ShaderProgram::Use()
{
	glUseProgram(this->ID);
}

void ShaderProgram::ConfigureProgram()
{
	SetShaderProgramID(glCreateProgram());

	shaders.AttachProgram(GetShaderProgramID());

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
	shaders.DeleteShaders();
}

unsigned int ShaderProgram::GetShaderProgramID() const
{
	return ID;
}

void ShaderProgram::SetShaderProgramID(unsigned int id)
{
	this->ID = id;
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(GetShaderProgramID(), name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(GetShaderProgramID(), name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(GetShaderProgramID(), name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(GetShaderProgramID(), name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(GetShaderProgramID(), name.c_str()), x, y);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(GetShaderProgramID(), name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(GetShaderProgramID(), name.c_str()), x, y, z);
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(GetShaderProgramID(), name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(GetShaderProgramID(), name.c_str()), x, y, z, w);
}

void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(GetShaderProgramID(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(GetShaderProgramID(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(GetShaderProgramID(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

