#include "OBB.h"


OBB::OBB(OBBConfig cfg)
    : AxisX(cfg.AxisX), AxisY(cfg.AxisY), AxisZ(cfg.AxisZ), HalfSize(cfg.HalfSize), shaderProgram(cfg.shaderProgram)
{
    this->pos = cfg.Pos;
    setUpVertices();
    setUpIndices();
    setUpDraw();
}

void OBB::Draw(glm::mat4 proj, glm::mat4 view)
{
    shaderProgram.Use();

    shaderProgram.setMat4("projection", proj);
    shaderProgram.setMat4("view", view);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = rotation * modelMatrix;
    shaderProgram.setMat4("model", modelMatrix);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.Size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool OBB::Intersects(const CollisionBoundingVolume& other) const
{
	return other.IntersectsOBB(*this);
}

bool OBB::IntersectsOBB(const OBB& other) const
{
    static glm::vec3 RPos;
    RPos = other.pos - this->pos;

    return !(getSeparatingPlane(RPos, this->AxisX, *this, other) ||
        getSeparatingPlane(RPos, this->AxisY, *this, other) ||
        getSeparatingPlane(RPos, this->AxisZ, *this, other) ||
        getSeparatingPlane(RPos, other.AxisX, *this, other) ||
        getSeparatingPlane(RPos, other.AxisY, *this, other) ||
        getSeparatingPlane(RPos, other.AxisZ, *this, other) ||
        getSeparatingPlane(RPos, glm::cross(this->AxisX, other.AxisX), *this, other) ||
        getSeparatingPlane(RPos, glm::cross(this->AxisX, other.AxisY), *this, other) ||
        getSeparatingPlane(RPos, glm::cross(this->AxisX, other.AxisZ), *this, other) ||
        getSeparatingPlane(RPos, glm::cross(this->AxisY, other.AxisX), *this, other) ||
        getSeparatingPlane(RPos, glm::cross(this->AxisY, other.AxisY), *this, other) ||
        getSeparatingPlane(RPos, glm::cross(this->AxisY, other.AxisZ), *this, other) ||
        getSeparatingPlane(RPos, glm::cross(this->AxisZ, other.AxisX), *this, other) ||
        getSeparatingPlane(RPos, glm::cross(this->AxisZ, other.AxisY), *this, other) ||
        getSeparatingPlane(RPos, glm::cross(this->AxisZ, other.AxisZ), *this, other));

	return false;
}

std::unique_ptr<CollisionBoundingVolume> OBB::clone() const
{
    return std::make_unique<OBB>(*this);
}

void OBB::UpdatePos(glm::vec3 pos)
{
        this->pos = pos;
}

void OBB::UpdateRotation(glm::mat4 rot)
{
    this->rotation = rot;
}

void OBB::UpdateScale(glm::vec3 scl)
{
    this->scale = scl;
}


bool OBB::getSeparatingPlane(const glm::vec3& RPos, const glm::vec3& Plane, const OBB& box1, const OBB& box2) const
{
    return (fabs(glm::dot(RPos, Plane)) >
        (fabs(glm::dot((box1.AxisX * box1.HalfSize.x), Plane)) +
            fabs(glm::dot((box1.AxisY * box1.HalfSize.y), Plane)) +
            fabs(glm::dot((box1.AxisZ * box1.HalfSize.z), Plane)) +
            fabs(glm::dot((box2.AxisX * box2.HalfSize.x), Plane)) +
            fabs(glm::dot((box2.AxisY * box2.HalfSize.y), Plane)) +
            fabs(glm::dot((box2.AxisZ * box2.HalfSize.z), Plane))));
}

void OBB::setUpDraw()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    this->vertices.SetupBuffer();
    indices.SetupEBO();
    glBindVertexArray(0);
}

void OBB::setUpVertices()
{
    VertexData vertex;
    vertex.Position = HalfSize.x * glm::normalize(AxisX) + HalfSize.y * glm::normalize(AxisY) + HalfSize.z * glm::normalize(AxisZ);
    vertices.AddVertice(vertex);
    vertex.Position = HalfSize.x * glm::normalize(AxisX) - HalfSize.y * glm::normalize(AxisY) + HalfSize.z * glm::normalize(AxisZ);
    vertices.AddVertice(vertex);
    vertex.Position = HalfSize.x * glm::normalize(AxisX) - HalfSize.y * glm::normalize(AxisY) - HalfSize.z * glm::normalize(AxisZ);
    vertices.AddVertice(vertex);
    vertex.Position = HalfSize.x * glm::normalize(AxisX) + HalfSize.y * glm::normalize(AxisY) - HalfSize.z * glm::normalize(AxisZ);
    vertices.AddVertice(vertex);
    vertex.Position =  -1 * HalfSize.x * glm::normalize(AxisX) + HalfSize.y * glm::normalize(AxisY) + HalfSize.z * glm::normalize(AxisZ);
    vertices.AddVertice(vertex);
    vertex.Position =  -1 * HalfSize.x * glm::normalize(AxisX) - HalfSize.y * glm::normalize(AxisY) + HalfSize.z * glm::normalize(AxisZ);
    vertices.AddVertice(vertex);
    vertex.Position =  -1 * HalfSize.x * glm::normalize(AxisX) - HalfSize.y * glm::normalize(AxisY) - HalfSize.z * glm::normalize(AxisZ);
    vertices.AddVertice(vertex);
    vertex.Position =  -1 * HalfSize.x * glm::normalize(AxisX) + HalfSize.y * glm::normalize(AxisY) - HalfSize.z * glm::normalize(AxisZ);
    vertices.AddVertice(vertex);
}

void OBB::setUpIndices()
{
    setUpTriangleIndices(0, 1, 2);
    setUpTriangleIndices(0, 2, 3);
    setUpTriangleIndices(4, 5, 6);
    setUpTriangleIndices(4, 6, 7);
    setUpTriangleIndices(3, 4, 7);
    setUpTriangleIndices(0, 3, 4);
    setUpTriangleIndices(3, 6, 7);
    setUpTriangleIndices(2, 3, 6);
    setUpTriangleIndices(1, 2, 6);
    setUpTriangleIndices(1, 5, 6);
    setUpTriangleIndices(0, 4, 5);
    setUpTriangleIndices(0, 1, 5);
}

void OBB::setUpTriangleIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.AddIndex(i1);
    indices.AddIndex(i2);
    indices.AddIndex(i3);
}
