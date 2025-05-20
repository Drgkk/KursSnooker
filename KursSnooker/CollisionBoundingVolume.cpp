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


//
//OBB::OBB(OBBConfig cfg)
//    : HalfSize(cfg.HalfSize)
//{
//    setUpVertices();
//    setUpIndices();
//}


//
//bool OBB::Intersects(const CollisionBoundingVolume& other) const
//{
//	return other.IntersectsOBB(*this);
//}
//
//bool OBB::IntersectsOBB(const OBB& other) const
//{
//    return false;
//}

bool OBB::Intersects(const CollisionBoundingVolume& other, CollisionData* collisionData) const
{
	return other.IntersectsOBB(*this, collisionData);
}

bool OBB::IntersectsOBB(const OBB& other, CollisionData* collisionData) const
{
	return CollisionDetector::OBBandOBB(other, *this, collisionData);
}

bool OBB::IntersectsHalfSpace(const CollisionPlane& plane, CollisionData* collisionData) const
{
	return CollisionDetector::OBBAndHalfSpace(*this, plane, collisionData);
}

bool OBB::IntersectsSphere(const CollisionSphere& other, CollisionData* collisionData) const
{
	return CollisionDetector::OBBAndSphere(*this, other, collisionData);
}


bool CollisionSphere::Intersects(const CollisionBoundingVolume& other, CollisionData* collisionData) const
{
	return other.IntersectsSphere(*this, collisionData);
}

bool CollisionSphere::IntersectsOBB(const OBB& other, CollisionData* collisionData) const
{
	return CollisionDetector::OBBAndSphere(other, *this, collisionData);
}

bool CollisionSphere::IntersectsHalfSpace(const CollisionPlane& plane, CollisionData* collisionData) const
{
	return CollisionDetector::SphereAndHalfSpace(*this, plane, collisionData);
}

bool CollisionSphere::IntersectsSphere(const CollisionSphere& other, CollisionData* collisionData) const
{
	return CollisionDetector::SphereAndSphere(other, *this, collisionData);
}
