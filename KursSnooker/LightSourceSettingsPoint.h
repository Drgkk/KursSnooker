#pragma once
#include "LightSourceSettings.h"

struct LightSourceSettingsPointConfig {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float constant;
	float linear;
	float quadratic;
	int index;
};

class LightSourceSettingsPoint : public LightSourceSettings
{
public:
	LightSourceSettingsPoint(LightSourceSettingsPointConfig cfg);
	void ApplyParameters(ShaderProgram& shaderProgram, glm::vec3 pos) override;
	std::unique_ptr<LightSourceSettings> clone() const;
private:
	float constant;
	float linear;
	float quadratic;
	int index;
};

