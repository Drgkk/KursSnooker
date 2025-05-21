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
	bool value = other.IntersectsOBB(*this, collisionData);
	if(value)
		this->body->Intersects(other.body.get());
	return value;
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

bool OBB::IntersectsRay(const glm::vec3 nearPt, const glm::vec3 farPt, glm::vec3& worldPoint) const
{
	return false;
}


bool CollisionSphere::Intersects(const CollisionBoundingVolume& other, CollisionData* collisionData) const
{
	bool value = other.IntersectsSphere(*this, collisionData);
	if (value) {
		this->body->Intersects(other.body.get());
		other.body->Intersects(this->body.get());
	}
		
	return value;
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

bool CollisionSphere::IntersectsRay(const glm::vec3 nearPt, const glm::vec3 farPt, glm::vec3& worldPoint) const
{
	return CollisionDetector::RayAndSphereWorld(nearPt, farPt, *this, worldPoint);
}
