#pragma once
#include <math.h>
#include "CollisionBoundingVolume.h"

class CollisionBoundingVolume;

class OBB : public CollisionBoundingVolume
{
public:
	//OBB(OBBConfig cfg);
	bool Intersects(const CollisionBoundingVolume& other, CollisionData* collisionData) const;
	bool IntersectsOBB(const class OBB& other, CollisionData* collisionData) const;
	virtual void Draw(glm::mat4 proj, glm::mat4 view) = 0;
	std::unique_ptr<CollisionBoundingVolume> clone() const override;
	glm::vec3 HalfSize;
private:
	
};

