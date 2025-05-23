#include "Box.h"

Box::Box(ShaderProgram& shaderProgram, glm::mat4 offset, glm::vec3 halfSize, std::shared_ptr<RigidBody> body) : shaderProgram(shaderProgram)
{
    this->offset = offset;
    this->HalfSize = halfSize;
    this->body = body;
    this->transform = offset;
    setUpVertices();
    this->CalculateInternals();
    setUpDraw();
}

void Box::Draw(glm::mat4 proj, glm::mat4 view)
{
    shaderProgram.Use();

    shaderProgram.setMat4("projection", proj);
    shaderProgram.setMat4("view", view);
    shaderProgram.setMat4("model", this->GetTransform());
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.Size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Box::setUpDraw()
{

    setUpIndices();
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    this->vertices.SetupBuffer();
    indices.SetupEBO();
    glBindVertexArray(0);
}

void Box::setUpVertices()
{
    VertexData vertex;
    vertex.Position = HalfSize.x * glm::normalize(GetAxis(0)) + HalfSize.y * glm::normalize(GetAxis(1)) + HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = HalfSize.x * glm::normalize(GetAxis(0)) - HalfSize.y * glm::normalize(GetAxis(1)) + HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = HalfSize.x * glm::normalize(GetAxis(0)) - HalfSize.y * glm::normalize(GetAxis(1)) - HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = HalfSize.x * glm::normalize(GetAxis(0)) + HalfSize.y * glm::normalize(GetAxis(1)) - HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = - HalfSize.x * glm::normalize(GetAxis(0)) + HalfSize.y * glm::normalize(GetAxis(1)) + HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position =  - HalfSize.x * glm::normalize(GetAxis(0)) - HalfSize.y * glm::normalize(GetAxis(1)) + HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position =  - HalfSize.x * glm::normalize(GetAxis(0)) - HalfSize.y * glm::normalize(GetAxis(1)) - HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position =  - HalfSize.x * glm::normalize(GetAxis(0)) + HalfSize.y * glm::normalize(GetAxis(1)) - HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
}

void Box::setUpIndices()
{
    setUpTriangleIndices(1, 2, 3);
    setUpTriangleIndices(2, 4, 3);
    setUpTriangleIndices(1, 4, 5);
    setUpTriangleIndices(1, 4, 8);
    setUpTriangleIndices(5, 8, 7);
    setUpTriangleIndices(5, 8, 6);
    setUpTriangleIndices(3, 7, 6);
    setUpTriangleIndices(2, 7, 6);
    setUpTriangleIndices(1, 6, 5);
    setUpTriangleIndices(1, 2, 5);
    setUpTriangleIndices(1, 6, 5);
    setUpTriangleIndices(3, 4, 7);
    setUpTriangleIndices(3, 4, 8);
}

void Box::setUpTriangleIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.AddIndex(i1 - 1);
    indices.AddIndex(i2 - 1);
    indices.AddIndex(i3 - 1);
}
