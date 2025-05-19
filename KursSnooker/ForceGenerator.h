#pragma once
#include "RigidBody.h"

class ForceGenerator
{
public:
	virtual void UpdateForce(RigidBody* body, float duration) = 0;
};

