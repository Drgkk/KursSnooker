#include "CollisionDetector.h"
#include <iostream>

static inline float transformToAxis(const OBB& box, const glm::vec3& axis)
{
    return
        box.HalfSize.x * fabs(glm::dot(axis, box.GetAxis(0))) +
        box.HalfSize.y * fabs(glm::dot(axis, box.GetAxis(1))) +
        box.HalfSize.z * fabs(glm::dot(axis, box.GetAxis(2)));
}

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

    if (one.body->isClippable || two.body->isClippable)
        return true;

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

unsigned int CollisionDetector::OBBAndHalfSpace(const OBB& box, const CollisionPlane& plane, CollisionData* collisionData)
{
    if (collisionData->contactsLeft <= 0) return false;
    if (!IntersectionsTest::BoxAndHalfSpace(box, plane)) {
        return 0;
    }

    static float mults[8][3] = { {1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
                               {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1} };

    std::vector<Contact>& contacts = collisionData->contacts;
    unsigned contactsUsed = 0;
    for (unsigned i = 0; i < 8; i++) {

        glm::vec3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
        vertexPos = vertexPos * box.HalfSize;
        vertexPos = glm::vec3(box.GetTransform() * glm::vec4(vertexPos, 1.0f));

        float vertexDistance = glm::dot(vertexPos, plane.direction);
        Contact contact;
        if (vertexDistance <= plane.offset)
        {
            if (box.body->isClippable)
                return 1;
            contact.contactPoint = plane.direction;
            contact.contactPoint *= (vertexDistance - plane.offset);
            contact.contactPoint += vertexPos;
            contact.contactNormal = plane.direction;
            contact.penetration = plane.offset - vertexDistance;

            contact.SetBodyData(box.body, NULL,
                collisionData->friction, collisionData->restitution);

            contacts.push_back(contact);
            contactsUsed++;
            if (contactsUsed == (unsigned)collisionData->contactsLeft) return contactsUsed;
        }
    }

    collisionData->addContacts(contactsUsed);
    return contactsUsed;
}

unsigned int CollisionDetector::SphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* collisionData)
{
    if (collisionData->contactsLeft <= 0) return 0;
    glm::vec3 position = sphere.GetAxis(3);
    float ballDistance = glm::dot(plane.direction, position) - sphere.radius - plane.offset;
    if (ballDistance >= 0) return 0;

    if (sphere.body->isClippable)
        return 1;

    Contact contact;
    contact.contactNormal = plane.direction;
    contact.penetration = -ballDistance;
    contact.contactPoint = position - plane.direction * (ballDistance + sphere.radius);
    contact.SetBodyData(sphere.body, NULL, collisionData->friction, collisionData->restitution);
    collisionData->contacts.push_back(contact);
    collisionData->addContacts(1);
    return 1;
}

unsigned int CollisionDetector::OBBAndSphere(const OBB& box, const CollisionSphere& sphere, CollisionData* collisionData)
{
    glm::vec3 centre = sphere.GetAxis(3);
    glm::vec3 relCentre = glm::vec3(glm::inverse(box.GetTransform()) * glm::vec4(centre, 1.0f));

    if (fabs(relCentre.x) - sphere.radius > box.HalfSize.x ||
        fabs(relCentre.y) - sphere.radius > box.HalfSize.y ||
        fabs(relCentre.z) - sphere.radius > box.HalfSize.z)
    {
        return 0;
    }

    glm::vec3 closestPt(0, 0, 0);
    float dist;

    dist = relCentre.x;
    if (dist > box.HalfSize.x) dist = box.HalfSize.x;
    if (dist < -box.HalfSize.x) dist = -box.HalfSize.x;
    closestPt.x = dist;

    dist = relCentre.y;
    if (dist > box.HalfSize.y) dist = box.HalfSize.y;
    if (dist < -box.HalfSize.y) dist = -box.HalfSize.y;
    closestPt.y = dist;

    dist = relCentre.z;
    if (dist > box.HalfSize.z) dist = box.HalfSize.z;
    if (dist < -box.HalfSize.z) dist = -box.HalfSize.z;
    closestPt.z = dist;

    glm::vec3 helpVec = (closestPt - relCentre);
    dist = glm::dot(helpVec, helpVec);
    if (dist > sphere.radius * sphere.radius) return 0;

    if (box.body->isClippable || sphere.body->isClippable)
        return 1;

    glm::vec3 closestPtWorld = glm::vec3(box.GetTransform() * glm::vec4(closestPt, 1.0f));

    Contact contact;
    contact.contactNormal = glm::normalize((closestPtWorld - centre));
    contact.contactPoint = closestPtWorld;
    contact.penetration = sphere.radius - sqrtf(dist);
    contact.SetBodyData(box.body, sphere.body,
        collisionData->friction, collisionData->restitution);

    collisionData->contacts.push_back(contact);
    collisionData->addContacts(1);
    return 1;
}

unsigned int CollisionDetector::SphereAndSphere(const CollisionSphere& one, const CollisionSphere& two, CollisionData* collisionData)
{
    if (collisionData->contactsLeft <= 0) return 0;

    glm::vec3 positionOne = one.GetAxis(3);
    glm::vec3 positionTwo = two.GetAxis(3);

    glm::vec3 midline = positionOne - positionTwo;
    float size = glm::length(midline);

    if (size <= 0.0f || size >= one.radius + two.radius)
    {
        return 0;
    }

    if (one.body->isClippable || two.body->isClippable)
        return 1;

    glm::vec3 normal = midline * ((1.0f) / size);

    Contact contact;
    contact.contactNormal = normal;
    contact.contactPoint = positionOne + midline * 0.5f;
    contact.penetration = (one.radius + two.radius - size);
    contact.SetBodyData(one.body, two.body,
        collisionData->friction, collisionData->restitution);

    collisionData->contacts.push_back(contact);
    collisionData->addContacts(1);
    return 1;
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



bool IntersectionsTest::BoxAndHalfSpace(const OBB& box, const CollisionPlane& plane)
{
    float projectedRadius = transformToAxis(box, plane.direction);
    float boxDistance = glm::dot(plane.direction, box.GetAxis(3)) - projectedRadius;
    return boxDistance <= plane.offset;

}

bool CollisionDetector::RayAndSphereWorld(glm::vec3 nearPt, glm::vec3 farPt, const CollisionSphere& sphere, glm::vec3& worldPoint)
{
    double a = pow((farPt.x - nearPt.x), 2) + pow((farPt.y - nearPt.y), 2) + pow((farPt.z - nearPt.z), 2);
    double b = 2 * ((farPt.x - nearPt.x) * (nearPt.x - sphere.body->GetPosition()[0])
        + (farPt.y - nearPt.y)*(nearPt.y - sphere.body->GetPosition()[1])
        + (farPt.z - nearPt.z)*(nearPt.z - sphere.body->GetPosition()[2]));
    double c = pow(nearPt.x - sphere.body->GetPosition()[0], 2) + pow(nearPt.y - sphere.body->GetPosition()[1], 2) + pow(nearPt.z - sphere.body->GetPosition()[2], 2) - pow(sphere.radius, 2);
    double delta = pow(b, 2) - 4 * a * c;
    double d;
    glm::vec3 ray = farPt - nearPt;


    if (delta < 0) {
        return false;
    }
    else if (delta == 0) {
        d = -b / (2 * a);
    }
    else {
        d = (-b - sqrt(delta)) / (2 * a);
    }
    glm::vec3 p = glm::vec3(nearPt.x + d * ray.x, nearPt.y + d * ray.y, nearPt.z + d * ray.z);
    worldPoint =  p;
    return true;
}

bool CollisionDetector::RayAndOBBWorld(glm::vec3 nearPt, glm::vec3 farPt, const OBB& box, glm::vec3& worldPoint)
{
    float tMin = 0.0f;
    float tMax = std::numeric_limits<float>::max();
    const float threshold = 0.00000001f;
    glm::mat4 inverse = glm::inverse(box.GetTransform());
    const glm::vec3 pos = box.offset[3];

    glm::vec3 start = glm::vec3(inverse * glm::vec4(nearPt, 1.0f));
    glm::vec3 end = glm::vec3(inverse * glm::vec4(farPt, 1.0f));
    const glm::vec3 delta = pos - start;

    glm::vec3 rayDirection = glm::normalize(end - start);

    glm::vec3 aabbMin = -1.0f * box.offset[0] * box.HalfSize.x - 1.0f * box.offset[1] * box.HalfSize.y - 1.0f * box.offset[2] * box.HalfSize.z;
    glm::vec3 aabbMax = 1.0f * box.offset[0] * box.HalfSize.x + 1.0f * box.offset[1] * box.HalfSize.y + 1.0f * box.offset[2] * box.HalfSize.z;


    glm::vec3 xAxis = box.offset[0];
    float e = glm::dot(xAxis, delta);
    float f = glm::dot(rayDirection, xAxis);
    if (fabs(f) > threshold) {
        float t1 = (e + aabbMin.x) / f;
        float t2 = (e + aabbMax.x) / f;
        if (t1 > t2) {
            const float w = t1;
            t1 = t2;
            t2 = w;
        }
        if (t2 < tMax)
            tMax = t2;
        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else {
        if (-e + aabbMin.x > 0.0f || -e + aabbMax.x < 0.0f)
            return false;
    }

    glm::vec3 yAxis = box.offset[1];
    e = glm::dot(yAxis, delta);
    f = glm::dot(rayDirection, yAxis);
    if (fabs(f) > threshold) {
        float t1 = (e + aabbMin.y) / f;
        float t2 = (e + aabbMax.y) / f;
        if (t1 > t2) {
            const float w = t1;
            t1 = t2;
            t2 = w;
        }
        if (t2 < tMax)
            tMax = t2;
        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else {
        if (-e + aabbMin.y > 0.0f || -e + aabbMax.y < 0.0f)
            return false;
    }


    glm::vec3 zAxis = box.offset[2];
    e = glm::dot(zAxis, delta);
    f = glm::dot(rayDirection, zAxis);
    if (fabs(f) > threshold) {
        float t1 = (e + aabbMin.z) / f;
        float t2 = (e + aabbMax.z) / f;
        if (t1 > t2) {
            const float w = t1;
            t1 = t2;
            t2 = w;
        }
        if (t2 < tMax)
            tMax = t2;
        if (t1 > tMin)
            tMin = t1;

        if (tMax < tMin)
            return false;
    }
    else {
        if (-e + aabbMin.z > 0.0f || -e + aabbMax.z < 0.0f)
            return false;
    }

    worldPoint = glm::vec3(box.GetTransform() * glm::vec4(start + rayDirection * tMin, 1.0f)) ;
    return true;
}
