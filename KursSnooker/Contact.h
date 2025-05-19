#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "ContactResolver.h"
#include "RigidBody.h"

class Contact
{
public:
	glm::vec3 contactPoint;
	glm::vec3 contactNormal;
	float penetration;
	std::shared_ptr<RigidBody> body[2];
	float friction;
	float restitution;
	void SetBodyData(std::shared_ptr<RigidBody> one, std::shared_ptr<RigidBody> two,
		float friction, float restitution);
	friend class ContactResolver;
protected:
	glm::mat3 contactToWorld;
	glm::vec3 contactVelocity;
	float desiredDeltaVelocity;
	glm::vec3 relativeContactPosition[2];

	void calculateInternals(float duration);
	void swapBodies();
	void matchAwakeState();
	void calculateDesiredDeltaVelocity(float duration);
	glm::vec3 calculateLocalVelocity(unsigned int bodyIndex, float duration);
	void calculateContactBasis();
	void applyImpulse(const glm::vec3 &impulse, RigidBody *body, glm::vec3 *velocityChange, glm::vec3 *rotationChange);
	void applyVelocityChange(glm::vec3 velocityChange[2], glm::vec3 rotationChange[2]);
	void applyPositionChange(glm::vec3 linearChange[2], glm::vec3 angularChange[2], float penetration);
	glm::vec3 calculateFrictionlessImpulse(glm::mat3* inverseInertiaTensor);
	glm::vec3 calculateFrictionImpulse(glm::mat3* inverseInertiaTensor);

};

