#include "ShadersBuilder.h"

ShadersBuilder::ShadersBuilder()
{
	this->Reset();
}

void ShadersBuilder::BuildVertexShader(const char* path)
{
	std::unique_ptr<Shader> vertexShader = std::make_unique<ShaderVertex>(path);
	vertexShader->Compile();
	shaders->AddShader(std::move(vertexShader));
}

void ShadersBuilder::BuildFragmentShader(const char* path)
{
	std::unique_ptr<Shader> fragmentShader = std::make_unique<ShaderFragment>(path);
	fragmentShader->Compile();
	shaders->AddShader(std::move(fragmentShader));
}

void ShadersBuilder::Reset()
{
	this->shaders = std::make_unique<Shaders>();
}

std::unique_ptr<Shaders> ShadersBuilder::Build()
{
	std::unique_ptr<Shaders> pointer = std::move(shaders);
	this->Reset();
	return std::move(pointer);
}
