#include "ForceRegistry.h"

void ForceRegistry::Add(RigidBody* body, ForceGenerator* fg)
{
	ForceRegistration registration;
	registration.body = body;
	registration.fg = fg;
	registrations.push_back(registration);
}

void ForceRegistry::Remove(RigidBody* body, ForceGenerator* fg)
{
	std::vector<ForceRegistration>::iterator i = registrations.begin();
	for (; i != registrations.end(); i++) {
		if (i->body == body && i->fg == fg) {
			registrations.erase(i);
			return;
		}
	}
	
}

void ForceRegistry::Clear()
{
	registrations.clear();
}

void ForceRegistry::UpdateForces(float duration)
{
	std::vector<ForceRegistration>::iterator i = registrations.begin();
	for (; i != registrations.end(); i++) {
		i->fg->UpdateForce(i->body, duration);
	}
}
