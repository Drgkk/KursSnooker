#include "BoundingVolumesBuilder.h"

void BoundingVolumesBuilder::Reset()
{
	this->boundingVolumes.clear();
}

std::vector<std::unique_ptr<CollisionBoundingVolume>> BoundingVolumesBuilder::Build()
{
	std::vector<std::unique_ptr<CollisionBoundingVolume>> vec = std::move(this->boundingVolumes);
	this->Reset();
	return std::move(vec);
}

void BoundingVolumesBuilder::BuildOBB(OBBConfig cfg)
{
	this->boundingVolumes.push_back(std::move(std::make_unique<OBB>(cfg)));
}
