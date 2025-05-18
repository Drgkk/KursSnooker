#include "LightSource.h"



LightSource::LightSource(LightSourceConfig cfg)
	: lightSprite(cfg.sprite), settings(std::move(cfg.settings))
{
}

LightSource::LightSource(const LightSource& other)
	: lightSprite(other.lightSprite)
{
	if (other.settings)
		settings = other.settings->clone();
	else
		settings = nullptr;
}

void LightSource::Draw(ShaderProgram& shaderProgram, float deltaTime, glm::mat4 proj, glm::mat4 view)
{
	ApplyParameters(shaderProgram);
	lightSprite.Draw(shaderProgram, deltaTime, proj, view);
}

void LightSource::ApplyParameters(ShaderProgram& shaderProgram)
{
	settings.get()->ApplyParameters(shaderProgram, lightSprite.GetPosition());
}
