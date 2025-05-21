#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Constants.h"

class RigidBody
{
public:
	void CalculateDerivedData();
	void Integrate(float duration);
	void SetMass(const float mass);
	float GetMass() const;
	void SetInverseMass(const float inverseMass);
	float GetInverseMass() const;
	bool HasFiniteMass() const;
	void SetInertiaTensor(const glm::mat3& inertiaTensor);
	void GetInertiaTensor(glm::mat3 *inertiaTensor) const;
	glm::mat3 GetInertiaTensor() const;
	void GetInertiaTensorWorld(glm::mat3 *inertiaTensor) const;
	glm::mat3 GetInertiaTensorWorld() const;
	void SetInverseInertiaTensor(const glm::mat3& inverseIntertiaTensor);
	void GetInverseInertiaTensor(glm::mat3* inverseInertiaTensor) const;
	glm::mat3 GetInverseInertiaTensor() const;
	void GetInverseInertiaTensorWorld(glm::mat3* inverseInertiaTensor) const;
	glm::mat3 GetInverseInertiaTensorWorld() const;
	void SetDamping(const float linearDamping, const float angularDamping);
	void SetLinearDamping(const float linearDamping);
	float GetLinearDamping() const;
	void SetAngularDamping(const float angularDamping);
	float GetAngularDamping() const;
	void SetPosition(const glm::vec3& position);
	void SetPosition(const float x, const float y, const float z);
	void GetPosition(glm::vec3* position) const;
	glm::vec3 GetPosition() const;
	void SetOrientation(const glm::quat& orientation);
	void SetOrientation(const float r, const float i,
		const float j, const float k);
	void GetOrientation(glm::quat* orientation) const;
	glm::quat GetOrientation() const;
	void GetOrientation(glm::mat3* matrix) const;
	void GetTransform(glm::mat4* transform) const;
	glm::mat4 GetTransform() const;
	glm::vec3 GetPointInLocalSpace(const glm::vec3& point) const;
	glm::vec3 GetPointInWorldSpace(const glm::vec3& point) const;
	glm::vec3 GetDirectionInLocalSpace(const glm::vec3& direction) const;
	glm::vec3 GetDirectionInWorldSpace(const glm::vec3& direction) const;
	void SetVelocity(const glm::vec3& velocity);
	void SetVelocity(const float x, const float y, const float z);
	void GetVelocity(glm::vec3* velocity) const;
	glm::vec3 GetVelocity() const;
	void AddVelocity(const glm::vec3& deltaVelocity);
	void SetRotation(const glm::vec3& rotation);
	void SetRotation(const float x, const float y, const float z);
	void GetRotation(glm::vec3* rotation) const;
	glm::vec3 GetRotation() const;
	void AddRotation(const glm::vec3& deltaRotation);
	bool GetAwake() const;
	void SetAwake(const bool awake = true);
	bool GetCanSleep() const;
	void SetCanSleep(const bool canSleep = true);
	void GetLastFrameAcceleration(glm::vec3* linearAcceleration) const;
	glm::vec3 GetLastFrameAcceleration() const;
	void ClearAccumulators();
	void AddForce(const glm::vec3& force);
	void AddForceAtPoint(const glm::vec3& force, const glm::vec3& point);
	void AddForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point);
	void AddTorque(const glm::vec3& torque);
	void SetAcceleration(const glm::vec3& acceleration);
	void SetAcceleration(const float x, const float y, const float z);
	void GetAcceleration(glm::vec3* acceleration) const;
	glm::vec3 GetAcceleration() const;
	void Intersects(RigidBody& other);
protected:
	float inverseMass;
	glm::mat3 inverseInertiaTensor;
	float linearDamping;
	float angularDamping;
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 velocity;
	glm::vec3 rotation;
	glm::mat3 inverseInertiaTensorWorld;
	float motion;
	bool isAwake;
	bool canSleep;
	glm::mat4 transformMatrix;
	glm::vec3 forceAccum;
	glm::vec3 torqueAccum;
	glm::vec3 acceleration;
	glm::vec3 lastFrameAcceleration;
	virtual void onContact(RigidBody& other) {};
};

