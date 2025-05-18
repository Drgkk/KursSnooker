#include "LightSourceSettingsPoint.h"

LightSourceSettingsPoint::LightSourceSettingsPoint(LightSourceSettingsPointConfig cfg)
	: constant(cfg.constant), linear(cfg.linear), quadratic(cfg.quadratic), index(cfg.index)
{
	ambient = cfg.ambient;
	diffuse = cfg.diffuse;
	specular = cfg.specular;
}

void LightSourceSettingsPoint::ApplyParameters(ShaderProgram& shaderProgram, glm::vec3 pos)
{
	std::string str = "pointLights[" + std::to_string(index) + "]";
	shaderProgram.setVec3(str + ".position", pos);
	shaderProgram.setVec3(str + ".ambient", ambient);
	shaderProgram.setVec3(str + ".diffuse", diffuse);
	shaderProgram.setVec3(str + ".specular", specular);
	shaderProgram.setFloat(str + ".constant", constant);
	shaderProgram.setFloat(str + ".linear", linear);
	shaderProgram.setFloat(str + ".quadratic", quadratic);
}

std::unique_ptr<LightSourceSettings> LightSourceSettingsPoint::clone() const
{
	return std::make_unique<LightSourceSettingsPoint>(*this);
}
