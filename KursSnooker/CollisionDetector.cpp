#include "CollisionDetector.h"


bool CollisionDetector::OBBandOBB(const OBB& one, const OBB& two, CollisionData* collisionData)
{
    float bestOverlap = std::numeric_limits<float>::max();
    int bestCase = std::numeric_limits<int>::max();
    glm::vec3 toCentre;
    toCentre = two.GetAxis(3) - one.GetAxis(3);


    glm::vec3 axes[15];
    axes[0] = one.GetAxis(0);
    axes[1] = one.GetAxis(1);
    axes[2] = one.GetAxis(2);

    axes[3] = two.GetAxis(0);
    axes[4] = two.GetAxis(1);
    axes[5] = two.GetAxis(2);

    unsigned int bestSingleAxis = bestCase;

    axes[6] = glm::cross(one.GetAxis(0), two.GetAxis(0));
    axes[7] = glm::cross(one.GetAxis(0), two.GetAxis(1));
    axes[8] = glm::cross(one.GetAxis(0), two.GetAxis(2));
    axes[9] = glm::cross(one.GetAxis(1), two.GetAxis(0));
    axes[10] = glm::cross(one.GetAxis(1), two.GetAxis(1));
    axes[11] = glm::cross(one.GetAxis(1), two.GetAxis(2));
    axes[12] = glm::cross(one.GetAxis(2), two.GetAxis(0));
    axes[13] = glm::cross(one.GetAxis(2), two.GetAxis(1));
    axes[14] = glm::cross(one.GetAxis(2), two.GetAxis(2));

    for (unsigned int index = 0; index < 15; index++) {
        glm::vec3 axis = axes[index];

        if (glm::dot(axis, axis) < 0.001) continue;
        axis = glm::normalize(axis);

        float overlap = penetrationOnAxis(toCentre, axis, one, two);
        if (overlap < 0) return false;
        if (overlap < bestOverlap) {
            bestOverlap = overlap;
            bestCase = index;
        }
    }

    if (bestCase == std::numeric_limits<int>::max()) {
        return false;
    }

    if (bestCase < 3) {
        fillPointFaceOBBOBB(one, two, toCentre, collisionData, bestCase, bestOverlap);
        collisionData->addContacts(1);
        return true;
    }
    else if (bestCase < 6) {
        fillPointFaceOBBOBB(two, one, toCentre*-1.0f, collisionData, bestCase - 3, bestOverlap);
        collisionData->addContacts(1);
        return true;
    }
    else {
        bestCase -= 6;
        unsigned int oneAxisIndex = bestCase / 3;
        unsigned int twoAxisIndex = bestCase % 3;
        glm::vec3 oneAxis = one.GetAxis(oneAxisIndex);
        glm::vec3 twoAxis = two.GetAxis(twoAxisIndex);
        glm::vec3 axis = glm::cross(oneAxis, twoAxis);
        axis = glm::normalize(axis);

        if (glm::dot(axis, toCentre) > 0) axis = axis * -1.0f;

        glm::vec3 ptOnOneEdge = one.HalfSize;
        glm::vec3 ptOnTwoEdge = two.HalfSize;

        for (unsigned int i = 0; i < 3; i++) {
            if (i == oneAxisIndex) ptOnOneEdge[i] = 0;
            else if (glm::dot(one.GetAxis(i), axis) > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

            if (i == twoAxisIndex) ptOnTwoEdge[i] = 0;
            else if (glm::dot(two.GetAxis(i), axis) < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
        }

        ptOnOneEdge = glm::vec3(one.GetTransform() * glm::vec4(ptOnOneEdge, 1.0f));
        ptOnTwoEdge = glm::vec3(two.GetTransform() * glm::vec4(ptOnTwoEdge, 1.0f));

        glm::vec3 vertex = getContactPoint(
            ptOnOneEdge, oneAxis, one.HalfSize[oneAxisIndex],
            ptOnTwoEdge, twoAxis, two.HalfSize[twoAxisIndex],
            bestSingleAxis > 2
        );

        Contact contact;
        contact.penetration = bestOverlap;
        contact.contactNormal = axis;
        contact.contactPoint = vertex;
        contact.SetBodyData(one.body, two.body, collisionData->friction, collisionData->restitution);

        collisionData->addContacts(1);
        return true;

    }

    return false;

}

float CollisionDetector::penetrationOnAxis(const glm::vec3& toCentre, const glm::vec3& axis, const OBB& one, const OBB& two)
{
    float oneProject = transformToAxis(one, axis);
    float twoProject = transformToAxis(two, axis);

    float distance = fabs(glm::dot(toCentre, axis));

    return oneProject + twoProject - distance;

}

void CollisionDetector::fillPointFaceOBBOBB(const OBB& one, const OBB& two, const glm::vec3 toCentre, CollisionData* data, int best, float pen)
{
    std::vector<Contact>& contactArray = data->contacts;
    glm::vec3 normal = one.GetAxis(best);

    if (glm::dot(one.GetAxis(best), toCentre) > 0) {
        normal = normal * -1.0f;
    }

    glm::vec3 vertex = two.HalfSize;
    if (glm::dot(two.GetAxis(0), normal) < 0) vertex.x = -vertex.x;
    if (glm::dot(two.GetAxis(1), normal) < 0) vertex.y = -vertex.y;
    if (glm::dot(two.GetAxis(2), normal) < 0) vertex.z = -vertex.z;

    Contact contact;
    contact.contactNormal = normal;
    contact.penetration = pen;
    contact.contactPoint = glm::vec3(two.GetTransform() * glm::vec4(vertex, 1.0f));
    contact.SetBodyData(one.body, two.body, data->friction, data->restitution);
    data->contacts.push_back(contact);
}

glm::vec3 CollisionDetector::getContactPoint(const glm::vec3& pOne, const glm::vec3& dOne, float oneSize, const glm::vec3& pTwo, const glm::vec3& dTwo, float twoSize, bool useOne)
{
    glm::vec3 toSt, cOne, cTwo;
    float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
    float denom, mua, mub;

    smOne = glm::dot(dOne, dOne);
    smTwo = glm::dot(dTwo, dTwo);
    dpOneTwo = glm::dot(dTwo, dOne);

    toSt = pOne - pTwo;
    dpStaOne = glm::dot(dOne, toSt);
    dpStaTwo = glm::dot(dTwo, toSt);

    denom = smOne * smTwo - dpOneTwo * dpOneTwo;

    if (fabs(denom) < 0.0001f) {
        return useOne ? pOne : pTwo;
    }

    mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
    mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

    if (mua > oneSize ||
        mua < -oneSize ||
        mub > twoSize ||
        mub < -twoSize)
    {
        return useOne ? pOne : pTwo;
    }
    else
    {
        cOne = pOne + dOne * mua;
        cTwo = pTwo + dTwo * mub;

        return cOne * 0.5f + cTwo * 0.5f;
    }
}

float CollisionDetector::transformToAxis(const OBB& box, const glm::vec3& axis)
{
    return
        box.HalfSize.x * fabs(glm::dot(axis, box.GetAxis(0))) +
        box.HalfSize.y * fabs(glm::dot(axis, box.GetAxis(1))) +
        box.HalfSize.z * fabs(glm::dot(axis, box.GetAxis(2)));
}
