#pragma once
#include <memory>
#include "RigidBody.h"
#include "CollisionDetector.h"

struct CollisionData;

class CollisionBoundingVolume
{
public:
	virtual ~CollisionBoundingVolume() = default;
	virtual bool Intersects(const CollisionBoundingVolume& other, CollisionData* collisionData) const = 0;
	virtual bool IntersectsOBB(const class OBB& other, CollisionData* collisionData) const = 0;
	virtual bool IntersectsHalfSpace(const class CollisionPlane& plane, CollisionData* collisionData) const = 0;
	virtual bool IntersectsSphere(const class CollisionSphere& other, CollisionData* collisionData) const = 0;
	virtual bool IntersectsRay(const glm::vec3 nearPt, const glm::vec3 farPt, glm::vec3& worldPoint) const = 0;
	virtual void Draw(glm::mat4 proj, glm::mat4 view) = 0;
	void CalculateInternals();
	const glm::mat4& GetTransform() const;
	glm::vec3 GetAxis(unsigned int index) const;
	glm::mat4 offset;
	std::shared_ptr<RigidBody> body;
protected:
	glm::mat4 transform;
};

class OBB : public CollisionBoundingVolume
{
public:
	//OBB(OBBConfig cfg);
	bool Intersects(const CollisionBoundingVolume& other, CollisionData* collisionData) const override;
	bool IntersectsOBB(const class OBB& other, CollisionData* collisionData) const override;
	bool IntersectsHalfSpace(const CollisionPlane& plane, CollisionData* collisionData) const override;
	bool IntersectsSphere(const class CollisionSphere& other, CollisionData* collisionData) const override;
	bool IntersectsRay(const glm::vec3 nearPt, const glm::vec3 farPt, glm::vec3& worldPoint) const override;
	virtual void Draw(glm::mat4 proj, glm::mat4 view) = 0;
	//std::unique_ptr<CollisionBoundingVolume> clone() const override;
	glm::vec3 HalfSize;
private:

};

class CollisionSphere : public CollisionBoundingVolume 
{
public:
	bool Intersects(const CollisionBoundingVolume& other, CollisionData* collisionData) const override;
	bool IntersectsOBB(const class OBB& other, CollisionData* collisionData) const override;
	bool IntersectsHalfSpace(const CollisionPlane& plane, CollisionData* collisionData) const override;
	bool IntersectsSphere(const class CollisionSphere& other, CollisionData* collisionData) const override;
	bool IntersectsRay(const glm::vec3 nearPt, const glm::vec3 farPt, glm::vec3& worldPoint) const override;
	virtual void Draw(glm::mat4 proj, glm::mat4 view) = 0;
	float radius;

};

class CollisionPlane {
public:
	glm::vec3 direction;
	float offset;
};

