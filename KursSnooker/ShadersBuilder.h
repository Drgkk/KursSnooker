#pragma once
#include "Shaders.h"
#include "Shader.h"
#include "ShaderVertex.h"
#include "ShaderFragment.h"

class ShadersBuilder
{
public:
	ShadersBuilder();
	void BuildVertexShader(const char* path);
	void BuildFragmentShader(const char* path);
	void Reset();
	std::unique_ptr<Shaders> Build();
private:
	std::unique_ptr<Shaders> shaders;
};

