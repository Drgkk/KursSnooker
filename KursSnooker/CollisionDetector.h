#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OBB.h"
#include "Contact.h"

class OBB;

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
	
private:
	static void fillPointFaceOBBOBB(const OBB& one, const OBB& two, const glm::vec3 toCentre, CollisionData* data, int best, float pen);
	static glm::vec3 getContactPoint(const glm::vec3& pOne, const glm::vec3& dOne, float oneSize,
		const glm::vec3& pTwo, const glm::vec3& dTwo, float twoSize, bool useOne);
	static float transformToAxis(const OBB& box, const glm::vec3& axis);
	static float penetrationOnAxis(const glm::vec3& RPos, const glm::vec3& Plane, const OBB& box1, const OBB& box2);
};

