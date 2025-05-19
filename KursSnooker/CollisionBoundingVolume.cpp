#include "CollisionBoundingVolume.h"

void CollisionBoundingVolume::CalculateInternals()
{
	this->transform = body->GetTransform() * offset;
}

const glm::mat4& CollisionBoundingVolume::GetTransform() const
{
	return this->transform;
}

glm::vec3 CollisionBoundingVolume::GetAxis(unsigned int index) const
{
	glm::vec4 v = this->transform[index];
	return glm::vec3(v);
}

