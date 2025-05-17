#include "LightSourceSettingsPoint.h"

LightSourceSettingsPoint::LightSourceSettingsPoint(LightSourceSettingsConfig cfg) : maxDistance(cfg.maxDistance)
{
}

void LightSourceSettingsPoint::ApplyParameters(ShaderProgram& shaderProgram)
{
	shaderProgram.setFloat("light.maxDistance", maxDistance);
}

std::unique_ptr<LightSourceSettings> LightSourceSettingsPoint::clone() const
{
	return std::make_unique<LightSourceSettingsPoint>(*this);
}
