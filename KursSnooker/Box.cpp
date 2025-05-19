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
    shaderProgram.setMat4("model", this->transform);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(VAO);
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
    vertex.Position = glm::vec3(this->offset[3]) + HalfSize.x * glm::normalize(GetAxis(0)) + HalfSize.y * glm::normalize(GetAxis(1)) + HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = glm::vec3(this->offset[3]) + HalfSize.x * glm::normalize(GetAxis(0)) - HalfSize.y * glm::normalize(GetAxis(1)) + HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = glm::vec3(this->offset[3]) + HalfSize.x * glm::normalize(GetAxis(0)) - HalfSize.y * glm::normalize(GetAxis(1)) - HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = glm::vec3(this->offset[3]) + HalfSize.x * glm::normalize(GetAxis(0)) + HalfSize.y * glm::normalize(GetAxis(1)) - HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = glm::vec3(this->offset[3]) - HalfSize.x * glm::normalize(GetAxis(0)) + HalfSize.y * glm::normalize(GetAxis(1)) + HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = glm::vec3(this->offset[3]) - HalfSize.x * glm::normalize(GetAxis(0)) - HalfSize.y * glm::normalize(GetAxis(1)) + HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = glm::vec3(this->offset[3]) - HalfSize.x * glm::normalize(GetAxis(0)) - HalfSize.y * glm::normalize(GetAxis(1)) - HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
    vertex.Position = glm::vec3(this->offset[3]) - HalfSize.x * glm::normalize(GetAxis(0)) + HalfSize.y * glm::normalize(GetAxis(1)) - HalfSize.z * glm::normalize(GetAxis(2));
    vertices.AddVertice(vertex);
}

void Box::setUpIndices()
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

void Box::setUpTriangleIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.AddIndex(i1);
    indices.AddIndex(i2);
    indices.AddIndex(i3);
}
