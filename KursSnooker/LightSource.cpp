#include "LightSource.h"



LightSource::LightSource(std::shared_ptr<Sprite> sprite, std::unique_ptr<LightSourceSettings> settings)
	: lightSprite(sprite), settings(std::move(settings))
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

void LightSource::Draw(float deltaTime, glm::mat4 proj, glm::mat4 view)
{
	ApplyParameters(lightSprite->GetShaderProgram());
	lightSprite->Draw(deltaTime, proj, view);
}

void LightSource::ApplyParameters(ShaderProgram& shaderProgram)
{
	settings.get()->ApplyParameters(shaderProgram, lightSprite->GetPosition());
}
