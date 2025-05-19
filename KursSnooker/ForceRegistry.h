#pragma once
#include <vector>
#include "RigidBody.h"
#include "ForceGenerator.h"

class ForceRegistry
{
public:
	void Add(RigidBody* body, ForceGenerator* fg);
	void Remove(RigidBody* body, ForceGenerator* fg);
	void Clear();
	void UpdateForces(float duration);
protected:
	struct ForceRegistration {
		RigidBody* body;
		ForceGenerator* fg;
	};

	std::vector<ForceRegistration> registrations;
};

