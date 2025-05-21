#include "Vertices.h"


void Vertices::SetupBuffer()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->verticesData.size() * sizeof(VertexData), &verticesData[0], GL_STATIC_DRAW);
	setVertexAttributePointers();
}

void Vertices::AddVertice(VertexData vertice)
{
	verticesData.push_back(vertice);
}


void Vertices::setVertexAttributePointers()
{
	setVertexPositions();
	setVertexNormals();
	setTextureCoords();
	setVertexTangent();
	setVertexBitangent();
	setIDs();
	setWeights();
}

void Vertices::setVertexPositions()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
}

void Vertices::setVertexNormals()
{
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Normal));
}

void Vertices::setTextureCoords()
{
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, TexCoords));
}

void Vertices::setVertexTangent()
{
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Tangent));
}

void Vertices::setVertexBitangent()
{
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Bitangent));
}

void Vertices::setIDs()
{
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, m_BoneIDs));
}

void Vertices::setWeights()
{
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, m_Weights));
}
