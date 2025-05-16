#include "Mesh.h"

Mesh::Mesh(Vertices vertices, Indices indices, Textures textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	setupMesh();
}

void Mesh::Draw(ShaderProgram& shaderProgram)
{
	this->textures.Bind(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.Size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	this->vertices.SetupBuffer();
	indices.SetupEBO();
	glBindVertexArray(0);
}