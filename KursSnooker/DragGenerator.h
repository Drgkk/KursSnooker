#pragma once
#include <iostream>
#include "ForceGenerator.h"

class DragGenerator : public ForceGenerator
{
public:
	DragGenerator(float k1, float k2);
	virtual void UpdateForce(RigidBody* body, float duration);
private:
	float k1;
	float k2;
};

