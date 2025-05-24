#include "DragGenerator.h"

DragGenerator::DragGenerator(float k1, float k2)
{
	this->k1 = k1;
	this->k2 = k2;
}

void DragGenerator::UpdateForce(RigidBody* body, float duration)
{
	glm::vec3 velocity = body->GetVelocity();
	if (velocity == glm::vec3(0.0f))
		return;
	float dragCoef = glm::length(velocity);
	dragCoef = k1 * dragCoef + k2 * dragCoef * dragCoef;
	velocity = glm::normalize(velocity);
	velocity = -dragCoef * velocity;
	body->AddForce(velocity);
}
