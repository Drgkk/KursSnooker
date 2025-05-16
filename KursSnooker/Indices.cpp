#include "Indices.h"

void Indices::AddIndex(unsigned int i)
{
	indices.push_back(i);
}

unsigned int Indices::Size()
{
	return indices.size();
}

void Indices::SetupEBO()
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}
