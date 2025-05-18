#pragma once
#include "ShaderProgram.h"


class LightSourceSettings
{
public:
	virtual ~LightSourceSettings() = default;
	virtual void ApplyParameters(ShaderProgram &shaderProgram, glm::vec3 pos) = 0;
	virtual std::unique_ptr<LightSourceSettings> clone() const = 0;
protected:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

