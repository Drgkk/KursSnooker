#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CollisionBoundingVolume.h"
#include "Contact.h"

class OBB;
class CollisionPlane;
class CollisionSphere;

class IntersectionsTest {
public:
	static bool BoxAndHalfSpace(const OBB& box, const CollisionPlane& plane);
};

struct CollisionData {
	std::vector<Contact> contacts;
	int contactsLeft;
	unsigned int contactCount;
	float friction;
	float restitution;
	float tolerance;
	bool hasMoreContacts() {
		return contactsLeft > 0;
	}
	void reset(unsigned maxContacts) {
		contactsLeft = maxContacts;
		contactCount = 0;
		contacts.clear();
	}
	void addContacts(unsigned int count) {
		contactsLeft -= count;
		contactCount += count;

	}
};

class CollisionDetector
{
public:
	static bool OBBandOBB(const OBB& one, const OBB& two, CollisionData* collisionData);
	static unsigned int OBBAndHalfSpace(const OBB& box, const CollisionPlane& plane, CollisionData* collisionData);
	static unsigned int SphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* collisionData);
	static unsigned int OBBAndSphere(const OBB& box, const CollisionSphere& sphere, CollisionData* collisionData);
	static unsigned int SphereAndSphere(const CollisionSphere& one, const CollisionSphere& two, CollisionData* collisionData);
	static bool RayAndSphereWorld(glm::vec3 nearPt, glm::vec3 farPt, const CollisionSphere& sphere, glm::vec3& worldPoint);
private:
	static void fillPointFaceOBBOBB(const OBB& one, const OBB& two, const glm::vec3 toCentre, CollisionData* data, int best, float pen);
	static glm::vec3 getContactPoint(const glm::vec3& pOne, const glm::vec3& dOne, float oneSize,
		const glm::vec3& pTwo, const glm::vec3& dTwo, float twoSize, bool useOne);
	static float penetrationOnAxis(const glm::vec3& RPos, const glm::vec3& Plane, const OBB& box1, const OBB& box2);
};

