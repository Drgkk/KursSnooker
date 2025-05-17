#pragma once
#include "ShaderProgram.h"

class LightSourceSettings
{
public:
	virtual ~LightSourceSettings() = default;
	virtual void ApplyParameters(ShaderProgram &shaderProgram) = 0;
	virtual std::unique_ptr<LightSourceSettings> clone() const = 0;
};

