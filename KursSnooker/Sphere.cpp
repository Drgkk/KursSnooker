#include "Sphere.h"

Sphere::Sphere(ShaderProgram& shaderProgram, glm::mat4 offset, float radius, std::shared_ptr<RigidBody> body)
	: shaderProgram(shaderProgram)
{
	this->radius = radius;
	this->offset = offset;
	this->body = body;
	this->transform = offset;

	this->CalculateInternals();
}

void Sphere::Draw(glm::mat4 proj, glm::mat4 view)
{
}

void Sphere::setUpDraw()
{
	setUpIndices();
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	this->vertices.SetupBuffer();
	indices.SetupEBO();
	glBindVertexArray(0);
}

void Sphere::setUpVertices()
{
}

void Sphere::setUpIndices()
{
}

void Sphere::setUpTriangleIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
}
