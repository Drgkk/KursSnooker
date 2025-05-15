#pragma once
#include "Shader.h"
class ShaderFragment : public Shader
{
public:
	using Shader::Shader;
protected:
	void CreateShader() override;
	void ThrowErrorMessage(char info[]) override;
};

