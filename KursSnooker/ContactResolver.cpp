#include "ContactResolver.h"
#include <iostream>

bool ContactResolver::IsValid()
{
    return (velocityIterations > 0) &&
        (positionIterations > 0) &&
        (velocityEpsilon >= 0.0f) &&
        (positionEpsilon >= 0.0f);
}

void ContactResolver::SetIterations(unsigned int velocityIterations, unsigned int positionIterations)
{
    this->velocityIterations = velocityIterations;
    this->positionIterations = positionIterations;
}

void ContactResolver::SetIterations(unsigned int iterations)
{
    SetIterations(iterations, iterations);
}

void ContactResolver::SetEpsilon(float velocityEpsilon, float positionEpsilon)
{
    this->velocityEpsilon = velocityEpsilon;
    this->positionEpsilon = positionEpsilon;
}

void ContactResolver::ResolveContacts(std::vector<Contact>& contactArray, unsigned int numContacts, float duration)
{
    if (numContacts == 0) return;
    if (!IsValid()) return;


    prepareContacts(contactArray, numContacts, duration);

    adjustPositions(contactArray, numContacts, duration);

    adjustVelocities(contactArray, numContacts, duration);

}

void ContactResolver::prepareContacts(std::vector<Contact>& contactArray, unsigned int numContacts, float duration)
{
    std::vector<Contact>::iterator lastContact = contactArray.end();
    for (auto i = contactArray.begin(); i != lastContact; i++) {
        i->calculateInternals(duration);
    }
}

void ContactResolver::adjustVelocities(std::vector<Contact>& contactArray, unsigned int numContacts, float duration)
{
    glm::vec3 velocityChange[2], rotationChange[2];
    glm::vec3 deltaVel;

    velocityIterationsUsed = 0;
    while (velocityIterationsUsed < velocityIterations)
    {
        float max = velocityEpsilon;
        unsigned int index = numContacts;
        for (unsigned int i = 0; i < numContacts; i++)
        {
            if (contactArray[i].desiredDeltaVelocity > max)
            {
                max = contactArray[i].desiredDeltaVelocity;
                index = i;
            }
        }
        if (index == numContacts) break;

        contactArray[index].matchAwakeState();

        contactArray[index].applyVelocityChange(velocityChange, rotationChange);

        for (unsigned int i = 0; i < numContacts; i++)
        {
            for (unsigned int b = 0; b < 2; b++) if (contactArray[i].body[b])
            {
                for (unsigned int d = 0; d < 2; d++)
                {
                    if (contactArray[i].body[b] == contactArray[index].body[d])
                    {
                        deltaVel = velocityChange[d] +
                            glm::cross(rotationChange[d], contactArray[i].relativeContactPosition[b]);

                        contactArray[i].contactVelocity +=  (glm::transpose(contactArray[i].contactToWorld) * deltaVel) * (b ? -1.0f : 1.0f);
                        contactArray[i].calculateDesiredDeltaVelocity(duration);
                    }
                }
            }
        }
        velocityIterationsUsed++;
    }
}

void ContactResolver::adjustPositions(std::vector<Contact>& contactArray, unsigned int numContacts, float duration)
{
    unsigned int i, index;
    glm::vec3 linearChange[2], angularChange[2];
    float max;
    glm::vec3 deltaPosition;

    positionIterationsUsed = 0;
    while (positionIterationsUsed < positionIterations)
    {
        max = positionEpsilon;
        index = numContacts;
        for (i = 0; i < numContacts; i++)
        {
            if (contactArray[i].penetration > max)
            {
                max = contactArray[i].penetration;
                index = i;
            }
        }
        if (index == numContacts) break;

        contactArray[index].matchAwakeState();

        contactArray[index].applyPositionChange(
            linearChange,
            angularChange,
            max);

        for (i = 0; i < numContacts; i++)
        {
            for (unsigned int b = 0; b < 2; b++) if (contactArray[i].body[b])
            {
                for (unsigned int d = 0; d < 2; d++)
                {
                    if (contactArray[i].body[b] == contactArray[index].body[d])
                    {
                        deltaPosition = linearChange[d] + glm::cross(angularChange[d], contactArray[i].relativeContactPosition[b]);

                        contactArray[i].penetration +=
                            glm::dot(deltaPosition, contactArray[i].contactNormal)
                            * (b ? 1.0f : -1.0f);
                    }
                }
            }
        }
        positionIterationsUsed++;
    }
}

ContactResolver::ContactResolver(unsigned int iterations, float velocityEpsilon, float positionEpsilon)
{
    SetIterations(iterations, iterations);
    SetEpsilon(velocityEpsilon, positionEpsilon);
}

ContactResolver::ContactResolver(unsigned int velocityIterations, unsigned int positionIterations, float velocityEpsilon, float positionEpsilon)
{
    SetIterations(velocityIterations, positionIterations);
    SetEpsilon(velocityEpsilon, positionEpsilon);
}


