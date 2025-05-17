#pragma once
#include "LightSourceSettings.h"

struct LightSourceSettingsConfig {
	float maxDistance;
};

class LightSourceSettingsPoint : public LightSourceSettings
{
public:
	LightSourceSettingsPoint(LightSourceSettingsConfig cfg);
	void ApplyParameters(ShaderProgram& shaderProgram) override;
	std::unique_ptr<LightSourceSettings> clone() const;
private:
	float maxDistance;
};

