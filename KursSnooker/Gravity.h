#pragma once
#include "ForceGenerator.h"

class Gravity : public ForceGenerator
{
public:
	Gravity(const glm::vec3 &gravity);
	virtual void UpdateForce(RigidBody* body, float duration);
private:
	glm::vec3 gravity;
};

