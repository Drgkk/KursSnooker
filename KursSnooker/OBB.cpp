#include "OBB.h"

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


std::unique_ptr<CollisionBoundingVolume> OBB::clone() const
{
    return std::make_unique<OBB>(*this);
}






