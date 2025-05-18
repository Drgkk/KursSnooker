#pragma once
#include <vector>
#include "CollisionBoundingVolume.h"
#include "OBB.h"

class BoundingVolumesBuilder
{
public:
	void Reset();
	std::vector<std::unique_ptr<CollisionBoundingVolume>> Build();
	void BuildOBB(OBBConfig cfg);
private:
	std::vector<std::unique_ptr<CollisionBoundingVolume>> boundingVolumes;
};

