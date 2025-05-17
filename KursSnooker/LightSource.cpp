#include "LightSource.h"



LightSource::LightSource(LightSourceConfig cfg)
	: lightSprite(cfg.sprite), ambient(cfg.ambient), diffuse(cfg.diffuse), specular(cfg.specular), intensity(cfg.intensity), settings(std::move(cfg.settings))
{
}

LightSource::LightSource(const LightSource& other)
	: lightSprite(other.lightSprite), ambient(other.ambient), diffuse(other.diffuse), specular(other.specular), intensity(other.intensity)
{
	if (other.settings)
		settings = other.settings->clone();
	else
		settings = nullptr;
}

void LightSource::Draw(ShaderProgram& shaderProgram)
{
	shaderProgram.setVec3("light.amb", ambient);
	shaderProgram.setVec3("light.dif", diffuse);
	shaderProgram.setVec3("light.spc", specular);
	shaderProgram.setFloat("light.intens", intensity);
	settings.get()->ApplyParameters(shaderProgram);
	lightSprite.Draw(shaderProgram, 0.0f);
}
