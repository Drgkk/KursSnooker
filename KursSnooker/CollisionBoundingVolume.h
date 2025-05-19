#pragma once
#include <memory>
#include "RigidBody.h"
#include "CollisionDetector.h"

class CollisionBoundingVolume
{
public:
	virtual ~CollisionBoundingVolume() = default;
	virtual bool Intersects(const CollisionBoundingVolume& other, CollisionData* collisionData) const = 0;
	virtual bool IntersectsOBB(const class OBB& other, CollisionData* collisionData) const = 0;
	virtual std::unique_ptr<CollisionBoundingVolume> clone() const = 0;
	virtual void Draw(glm::mat4 proj, glm::mat4 view) = 0;
	/*virtual void UpdatePos(glm::vec3 pos) = 0;
	virtual void UpdateRotation(glm::mat4 rot) = 0;
	virtual void UpdateScale(glm::vec3 scl) = 0;*/
	void CalculateInternals();
	const glm::mat4& GetTransform() const;
	glm::vec3 GetAxis(unsigned int index) const;
	glm::mat4 offset;
	std::shared_ptr<RigidBody> body;
protected:
	glm::mat4 transform;
};

