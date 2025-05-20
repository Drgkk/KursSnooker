#include "Gravity.h"

Gravity::Gravity(const glm::vec3& gravity) : gravity(gravity)
{
}

void Gravity::UpdateForce(RigidBody* body, float duration)
{
	if (!body->HasFiniteMass()) return;

	if(body->GetAwake())
		body->AddForce(gravity * body->GetMass());
}
