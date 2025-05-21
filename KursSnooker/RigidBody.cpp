#include "RigidBody.h"

static inline void _transformInertiaTensor(glm::mat3& iitWorld,
    const glm::quat& q,
    const glm::mat3& iitBody,
    const glm::mat4& rotmat)
{
    glm::mat3 R = glm::mat3_cast(q);
    iitWorld =  R * iitBody * glm::transpose(R);
}

static inline void _calculateTransformMatrix(glm::mat4& transformMatrix,
    const glm::vec3& position,
    const glm::quat& orientation)
{
    transformMatrix = glm::mat4_cast(orientation);
    transformMatrix[3] = glm::vec4(position, 1.0f);
}

static inline void _quaternionAddVector(glm::quat& quat, const glm::vec3 vec) {
    glm::quat q(0.0f, vec.x, vec.y, vec.z);
    q = q * quat;
    quat = quat + (q * 0.5f);
}

bool RigidBody::GetAwake() const
{
    return isAwake;
}

void RigidBody::SetAwake(const bool awake)
{
    if (awake) {
        isAwake = true;
        motion = Constants::sleepEpsilon * 2.0f;
    }
    else {
        isAwake = false;
        this->velocity = glm::vec3(0.0f);
        this->rotation = glm::vec3(0.0f);
    }
}

bool RigidBody::GetCanSleep() const
{
    return canSleep;
}

void RigidBody::SetCanSleep(const bool canSleep)
{
    this->canSleep = canSleep;
    if (!canSleep && !isAwake) SetAwake();
}

void RigidBody::GetLastFrameAcceleration(glm::vec3* linearAcceleration) const
{
    *linearAcceleration = lastFrameAcceleration;
}

glm::vec3 RigidBody::GetLastFrameAcceleration() const
{
    return this->lastFrameAcceleration;
}

void RigidBody::ClearAccumulators()
{
    this->forceAccum = glm::vec3(0.0f);
    this->torqueAccum = glm::vec3(0.0f);
}

void RigidBody::AddForce(const glm::vec3& force)
{
    forceAccum = forceAccum + force;
    isAwake = true;
}

void RigidBody::AddForceAtPoint(const glm::vec3& force, const glm::vec3& point)
{
    glm::vec3 pt = point;
    pt = pt - position;

    forceAccum = forceAccum + force;
    torqueAccum = torqueAccum + (glm::cross(pt, force));

    isAwake = true;
}

void RigidBody::AddForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point)
{
    glm::vec3 pt = GetPointInWorldSpace(point);
    AddForceAtPoint(force, pt);
}

void RigidBody::AddTorque(const glm::vec3& torque)
{
    this->torqueAccum = this->torqueAccum + torque;
    isAwake = true;
}

void RigidBody::SetAcceleration(const glm::vec3& acceleration)
{
    this->acceleration = acceleration;
}

void RigidBody::SetAcceleration(const float x, const float y, const float z)
{
    this->acceleration.x = x;
    this->acceleration.y = y;
    this->acceleration.z = z;
}

void RigidBody::GetAcceleration(glm::vec3* acceleration) const
{
    *acceleration = this->acceleration;
}

glm::vec3 RigidBody::GetAcceleration() const
{
    return this->acceleration;
}

void RigidBody::Intersects(RigidBody& other)
{
    this->onContact(other);
}

void RigidBody::CalculateDerivedData()
{
    orientation = glm::normalize(orientation);

    _calculateTransformMatrix(transformMatrix, position, orientation);


    _transformInertiaTensor(inverseInertiaTensorWorld,
        orientation,
        inverseInertiaTensor,
        transformMatrix);
}

void RigidBody::Integrate(float duration)
{
    if (!isAwake) return;

    lastFrameAcceleration = acceleration;
    lastFrameAcceleration = lastFrameAcceleration + (forceAccum * inverseMass);

    glm::vec3 angularAcceleration = inverseInertiaTensorWorld * torqueAccum;

    velocity = velocity + (lastFrameAcceleration * duration);

    rotation = rotation +(angularAcceleration * duration);

    velocity *= powf(linearDamping, duration);
    rotation *= powf(angularDamping, duration);

    position = position + (velocity * duration);

    _quaternionAddVector(orientation, rotation * duration);

    CalculateDerivedData();

    ClearAccumulators();

    if (canSleep) {
        float currentMotion = glm::dot(velocity, velocity) +
            glm::dot(rotation, rotation);

        float bias = powf(0.5, duration);
        motion = bias * motion + (1 - bias) * currentMotion;
        
        if (motion < Constants::sleepEpsilon) SetAwake(false);
        else if (motion > 10 * Constants::sleepEpsilon) motion = 10 * Constants::sleepEpsilon;
    }
}

void RigidBody::SetMass(const float mass)
{
    if (mass != 0) {
        inverseMass = (1.0f / mass);
    }
}

float RigidBody::GetMass() const
{
    if (inverseMass == 0)
        return std::numeric_limits<float>::max();
    else
        return (1.0f / inverseMass);
}

void RigidBody::SetInverseMass(const float inverseMass)
{
    this->inverseMass = inverseMass;
}

float RigidBody::GetInverseMass() const
{
    return inverseMass;
}

bool RigidBody::HasFiniteMass() const
{
    return inverseMass>=0.0f;
}

void RigidBody::SetInertiaTensor(const glm::mat3& inertiaTensor)
{
    inverseInertiaTensor = glm::inverse(inertiaTensor);
}

void RigidBody::GetInertiaTensor(glm::mat3* inertiaTensor) const
{
    *inertiaTensor = glm::inverse(this->inverseInertiaTensor);
}

glm::mat3 RigidBody::GetInertiaTensor() const
{
    return glm::inverse(this->inverseInertiaTensor);
}

void RigidBody::GetInertiaTensorWorld(glm::mat3* inertiaTensor) const
{
    *inertiaTensor = glm::inverse(this->inverseInertiaTensorWorld);
}

glm::mat3 RigidBody::GetInertiaTensorWorld() const
{
    return glm::inverse(this->inverseInertiaTensorWorld);
}

void RigidBody::SetInverseInertiaTensor(const glm::mat3& inverseIntertiaTensor)
{
    this->inverseInertiaTensor = inverseIntertiaTensor;
}

void RigidBody::GetInverseInertiaTensor(glm::mat3* inverseInertiaTensor) const
{
    *inverseInertiaTensor = this->inverseInertiaTensor;
}

glm::mat3 RigidBody::GetInverseInertiaTensor() const
{
    return this->inverseInertiaTensor;
}

void RigidBody::GetInverseInertiaTensorWorld(glm::mat3* inverseInertiaTensor) const
{
    *inverseInertiaTensor = this->inverseInertiaTensorWorld;
}

glm::mat3 RigidBody::GetInverseInertiaTensorWorld() const
{
    return this->inverseInertiaTensorWorld;
}

void RigidBody::SetDamping(const float linearDamping, const float angularDamping)
{
    this->linearDamping = linearDamping;
    this->angularDamping = angularDamping;
}

void RigidBody::SetLinearDamping(const float linearDamping)
{
    this->linearDamping = linearDamping;
}

float RigidBody::GetLinearDamping() const
{
    return this->linearDamping;
}

void RigidBody::SetAngularDamping(const float angularDamping)
{
    this->angularDamping = angularDamping;
}

float RigidBody::GetAngularDamping() const
{
    return this->angularDamping;
}

void RigidBody::SetPosition(const glm::vec3& position)
{
    this->position = position;
}

void RigidBody::SetPosition(const float x, const float y, const float z)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

void RigidBody::GetPosition(glm::vec3* position) const
{
    *position = this->position;
}

glm::vec3 RigidBody::GetPosition() const
{
    return this->position;
}

void RigidBody::SetOrientation(const glm::quat& orientation)
{
    this->orientation = orientation;
    this->orientation = glm::normalize(this->orientation);
}

void RigidBody::SetOrientation(const float r, const float i, const float j, const float k)
{
    glm::quat q(r, i, j, k);
    this->orientation = glm::normalize(q);
}

void RigidBody::GetOrientation(glm::quat* orientation) const
{
    *orientation = this->orientation;
}

glm::quat RigidBody::GetOrientation() const
{
    return this->orientation;
}

void RigidBody::GetOrientation(glm::mat3* matrix) const
{
    *matrix = glm::mat3_cast(this->orientation);
}

void RigidBody::GetTransform(glm::mat4* transform) const
{
    *transform = this->transformMatrix;
}

glm::mat4 RigidBody::GetTransform() const
{
    return this->transformMatrix;
}

glm::vec3 RigidBody::GetPointInLocalSpace(const glm::vec3& point) const
{
    return glm::vec3(glm::inverse(transformMatrix) * glm::vec4(point, 1.0f));
}

glm::vec3 RigidBody::GetPointInWorldSpace(const glm::vec3& point) const
{
    return glm::vec3(transformMatrix * glm::vec4(point, 1.0f));
}

glm::vec3 RigidBody::GetDirectionInLocalSpace(const glm::vec3& direction) const
{
    return glm::vec3(glm::inverse(transformMatrix) * glm::vec4(direction, 0.0f));;
}

glm::vec3 RigidBody::GetDirectionInWorldSpace(const glm::vec3& direction) const
{
    return glm::vec3(transformMatrix * glm::vec4(direction, 0.0f));
}

void RigidBody::SetVelocity(const glm::vec3& velocity)
{
    this->velocity = velocity;
}

void RigidBody::SetVelocity(const float x, const float y, const float z)
{
    this->velocity.x = x;
    this->velocity.y = y;
    this->velocity.z = z;
}

void RigidBody::GetVelocity(glm::vec3* velocity) const
{
    *velocity = this->velocity;
}

glm::vec3 RigidBody::GetVelocity() const
{
    return this->velocity;
}

void RigidBody::AddVelocity(const glm::vec3& deltaVelocity)
{
    this->velocity = this->velocity + deltaVelocity;
}

void RigidBody::SetRotation(const glm::vec3& rotation)
{
    this->rotation = rotation;
}

void RigidBody::SetRotation(const float x, const float y, const float z)
{
    this->rotation.x = x;
    this->rotation.y = y;
    this->rotation.z = z;
}

void RigidBody::GetRotation(glm::vec3* rotation) const
{
    *rotation = this->rotation;
}

glm::vec3 RigidBody::GetRotation() const
{
    return this->rotation;
}

void RigidBody::AddRotation(const glm::vec3& deltaRotation)
{
    this->rotation = this->rotation + deltaRotation;
}





