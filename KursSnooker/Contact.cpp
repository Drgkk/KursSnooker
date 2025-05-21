#include "Contact.h"

void Contact::SetBodyData(std::shared_ptr<RigidBody> one, std::shared_ptr<RigidBody> two, float friction, float restitution)
{
	this->body[0] = one;
	this->body[1] = two;
	this->friction = friction;
	this->restitution = restitution;
}

void Contact::calculateInternals(float duration)
{
	if (!body[0]) swapBodies();
	if (!body[0]) return;

	calculateContactBasis();


	relativeContactPosition[0] = contactPoint - body[0]->GetPosition();
	if (body[1]) {
		relativeContactPosition[1] = contactPoint - body[1]->GetPosition();
	}

	contactVelocity = calculateLocalVelocity(0, duration);
	if (body[1]) {
		contactVelocity -= calculateLocalVelocity(1, duration);
	}

	calculateDesiredDeltaVelocity(duration);
}

void Contact::swapBodies()
{
	contactNormal = contactNormal * -1.0f;

	std::shared_ptr<RigidBody> temp = body[0];
	body[0] = body[1];
	body[1] = temp;
}

void Contact::matchAwakeState()
{
	if (!body[1]) return;
	if (!body[0]) return;

	bool body0awake = body[0]->GetAwake();
	bool body1awake = body[1]->GetAwake();

	if (body0awake ^ body1awake) {
		if (body0awake) body[1]->SetAwake();
		else body[0]->SetAwake();
	}
}

void Contact::calculateDesiredDeltaVelocity(float duration)
{
	const static float velocityLimit = (float)0.25f;

	float velocityFromAcc = 0;

	if (body[0]->GetAwake()) {
		velocityFromAcc += glm::dot(body[0]->GetLastFrameAcceleration() * duration, contactNormal);
	}

	if (body[1] && body[1]->GetAwake()) {
		velocityFromAcc -= glm::dot(body[1]->GetLastFrameAcceleration() * duration, contactNormal);
	}
	float thisRestitution = restitution;
	if (fabs(contactVelocity.x) < velocityLimit) {
		thisRestitution = 0.0f;
	}

	desiredDeltaVelocity = -contactVelocity.x - thisRestitution * (contactVelocity.x - velocityFromAcc);

}

glm::vec3 Contact::calculateLocalVelocity(unsigned int bodyIndex, float duration)
{
	std::shared_ptr<RigidBody> thisBody = body[bodyIndex];
	glm::vec3 velocity = glm::cross(thisBody->GetRotation(), relativeContactPosition[bodyIndex]);
	velocity = velocity + thisBody->GetVelocity();

	glm::vec3 contactVelocity = glm::transpose(contactToWorld) * velocity;
	glm::vec3 accVelocity = thisBody->GetLastFrameAcceleration() * duration;
	accVelocity = glm::transpose(contactToWorld) * accVelocity;

	accVelocity.x = 0;

	contactVelocity = contactVelocity + accVelocity;

	return contactVelocity;
}

inline void Contact::calculateContactBasis()
{
	glm::vec3 contactTangent[2];

	if (fabs(contactNormal.x) > fabs(contactNormal.y)) {
		const float s = (float)1.0f / sqrtf(contactNormal.z * contactNormal.z + contactNormal.x * contactNormal.x);

		contactTangent[0].x = contactNormal.z * s;
		contactTangent[0].y = 0.0f;
		contactTangent[0].z = -contactNormal.x * s;

		contactTangent[1].x = contactNormal.y * contactTangent[0].x;
		contactTangent[1].y = contactNormal.z * contactTangent[0].x -
			contactNormal.x * contactTangent[0].z;
		contactTangent[1].z = -contactNormal.y * contactTangent[0].x;
	}
	else {
		const float s = (float)1.0f / sqrtf(contactNormal.z * contactNormal.z +
			contactNormal.y * contactNormal.y);

		contactTangent[0].x = 0;
		contactTangent[0].y = -contactNormal.z * s;
		contactTangent[0].z = contactNormal.y * s;

		contactTangent[1].x = contactNormal.y * contactTangent[0].z -
			contactNormal.z * contactTangent[0].y;
		contactTangent[1].y = -contactNormal.x * contactTangent[0].z;
		contactTangent[1].z = contactNormal.x * contactTangent[0].y;
	}

	contactToWorld = glm::mat3(contactNormal, contactTangent[0], contactTangent[1]);
}

void Contact::applyVelocityChange(glm::vec3 velocityChange[2], glm::vec3 rotationChange[2])
{
	glm::mat3 inverseInertiaTensor[2];
	body[0]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[0]);
	if (body[1])
		body[1]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[1]);

	glm::vec3 impulseContact;
	if (friction == (float)0.0f) {
		impulseContact = calculateFrictionlessImpulse(inverseInertiaTensor);
	}
	else {
		impulseContact = calculateFrictionImpulse(inverseInertiaTensor);
	}


	glm::vec3 impulse = contactToWorld * impulseContact;

	glm::vec3 impulsiveTorque = glm::cross(relativeContactPosition[0], impulse);
	rotationChange[0] = inverseInertiaTensor[0] * impulsiveTorque;
	velocityChange[0] = glm::vec3(0.0f);
	velocityChange[0] = velocityChange[0] + (impulse * body[0]->GetInverseMass());

	body[0]->AddVelocity(velocityChange[0]);
	body[0]->AddRotation(rotationChange[0]);

	if (body[1]) {
		glm::vec3 impulsiveTorque = glm::cross(impulse, relativeContactPosition[1]);
		rotationChange[1] = inverseInertiaTensor[1] * impulsiveTorque;
		velocityChange[1] = glm::vec3(0.0f);
		velocityChange[1] = velocityChange[1] + (impulse * (-1 * body[1]->GetInverseMass()));

		body[1]->AddVelocity(velocityChange[1]);
		body[1]->AddRotation(rotationChange[1]);
	}

}

static inline void _quaternionAddVector(glm::quat& quat, const glm::vec3 vec) {
	glm::quat q(0.0f, vec.x, vec.y, vec.z);
	q = q * quat;
	quat = quat + (q * 0.5f);
}

void Contact::applyPositionChange(glm::vec3 linearChange[2], glm::vec3 angularChange[2], float penetration)
{
	const float angularLimit = 0.2f;
	float angularMove[2];
	float linearMove[2];

	float totalInertia = 0;
	float linearInertia[2];
	float angularInertia[2];

	for (unsigned i = 0; i < 2; i++) if (body[i])
	{
		glm::mat3 inverseInertiaTensor;
		body[i]->GetInverseInertiaTensorWorld(&inverseInertiaTensor);

		glm::vec3 angularInertiaWorld =
			glm::cross(relativeContactPosition[i], contactNormal);
		angularInertiaWorld =
			inverseInertiaTensor * angularInertiaWorld;
		angularInertiaWorld =
			glm::cross(angularInertiaWorld, relativeContactPosition[i]);
		angularInertia[i] =
			glm::dot(angularInertiaWorld, contactNormal);

		linearInertia[i] = body[i]->GetInverseMass();

		totalInertia += linearInertia[i] + angularInertia[i];

	}

	for (unsigned i = 0; i < 2; i++) if (body[i])
	{
		float sign = (i == 0) ? 1.0f : -1.0f;
		angularMove[i] =
			sign * penetration * (angularInertia[i] / totalInertia);
		linearMove[i] =
			sign * penetration * (linearInertia[i] / totalInertia);

		glm::vec3 projection = relativeContactPosition[i];
		projection += contactNormal * glm::dot(-relativeContactPosition[i], contactNormal);

		float maxMagnitude = angularLimit * glm::length(projection);

		if (angularMove[i] < -maxMagnitude)
		{
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = -maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}
		else if (angularMove[i] > maxMagnitude)
		{
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}

		if (angularMove[i] == 0)
		{
			angularChange[i] = glm::vec3(0.0f);
		}
		else
		{
			glm::vec3 targetAngularDirection =
				glm::cross(relativeContactPosition[i], contactNormal);

			glm::mat3 inverseInertiaTensor;
			body[i]->GetInverseInertiaTensorWorld(&inverseInertiaTensor);

			angularChange[i] =
				(inverseInertiaTensor * targetAngularDirection) *
				(angularMove[i] / angularInertia[i]);
		}

		linearChange[i] = contactNormal * linearMove[i];

		glm::vec3 pos;
		body[i]->GetPosition(&pos);
		pos += contactNormal * linearMove[i];
		body[i]->SetPosition(pos);

		glm::quat q;
		body[i]->GetOrientation(&q);
		_quaternionAddVector(q, angularChange[i] * (1.0f));
		body[i]->SetOrientation(q);

		if (!body[i]->GetAwake()) body[i]->CalculateDerivedData();
	}

}

glm::vec3 Contact::calculateFrictionlessImpulse(glm::mat3* inverseInertiaTensor)
{
	glm::vec3 impulseContact;

	glm::vec3 deltaVelWorld = glm::cross(relativeContactPosition[0], contactNormal);
	deltaVelWorld = inverseInertiaTensor[0] * deltaVelWorld;
	deltaVelWorld = glm::cross(deltaVelWorld, relativeContactPosition[0]);

	float deltaVelocity = glm::dot(deltaVelWorld, contactNormal);
	deltaVelocity += body[0]->GetInverseMass();

	if (body[1]) {
		glm::vec3 deltaVelWorld = glm::cross(relativeContactPosition[1], contactNormal);
		deltaVelWorld = inverseInertiaTensor[1] * deltaVelWorld;
		deltaVelWorld = glm::cross(deltaVelWorld, relativeContactPosition[1]);

		deltaVelocity += glm::dot(deltaVelWorld, contactNormal);
		deltaVelocity += body[1]->GetInverseMass();
	}

	impulseContact.x = desiredDeltaVelocity / deltaVelocity;
	impulseContact.y = 0;
	impulseContact.z = 0;

	return impulseContact;
}

inline glm::mat3 skewSymmetric(const glm::vec3& v) {
	//TRANSPOSE HERE TEST
	return glm::transpose(glm::mat3(
		0.0f, v.z, -v.y,
		-v.z, 0.0f, v.x,
		v.y, -v.x, 0.0f
	));
}

glm::vec3 Contact::calculateFrictionImpulse(glm::mat3* inverseInertiaTensor)
{
	glm::vec3 impulseContact;
	float inverseMass = body[0]->GetInverseMass();

	glm::mat3 impulseToTorque = skewSymmetric(relativeContactPosition[0]);

	glm::mat3 deltaVelWorld = impulseToTorque;
	deltaVelWorld *= inverseInertiaTensor[0];
	deltaVelWorld *= impulseToTorque;
	deltaVelWorld *= -1;

	if (body[1]) {
		impulseToTorque = skewSymmetric(relativeContactPosition[1]);

		glm::mat3 deltaVelWorld2 = impulseToTorque;
		deltaVelWorld2 *= inverseInertiaTensor[1];
		deltaVelWorld2 *= impulseToTorque;
		deltaVelWorld2 *= -1;

		deltaVelWorld += deltaVelWorld2;

		inverseMass += body[1]->GetInverseMass();

	}

	glm::mat3 deltaVelocity = glm::transpose(contactToWorld);
	deltaVelocity *= deltaVelWorld;
	deltaVelocity *= contactToWorld;

	deltaVelocity[0][0] += inverseMass;
	deltaVelocity[1][1] += inverseMass;
	deltaVelocity[2][2] += inverseMass;

	glm::mat3 impulseMatrix = glm::inverse(deltaVelocity);

	glm::vec3 velKill(desiredDeltaVelocity, -contactVelocity.y, -contactVelocity.z);

	impulseContact = impulseMatrix * velKill;

	float planarImpulse = sqrtf(
		impulseContact.y * impulseContact.y +
		impulseContact.z * impulseContact.z
	);

	if (planarImpulse > impulseContact.x * friction)
	{

		impulseContact.y = impulseContact.y / planarImpulse;
		impulseContact.z = impulseContact.z / planarImpulse;

		impulseContact.x = deltaVelocity[0][0] +
			deltaVelocity[1][0] * friction * impulseContact.y +
			deltaVelocity[2][0] * friction * impulseContact.z;
		impulseContact.x = desiredDeltaVelocity / impulseContact.x;
		impulseContact.y = impulseContact.y * friction * impulseContact.x;
		impulseContact.z = impulseContact.z * friction * impulseContact.x;
	}
	return impulseContact;
}
