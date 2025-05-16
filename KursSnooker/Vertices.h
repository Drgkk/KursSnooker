#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_BONE_INFLUENCE 4

struct VertexData {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

class Vertices
{
public:

	void SetupBuffer();
	void AddVertice(VertexData vertice);
private:
	std::vector<VertexData> verticesData;
	unsigned int VBO;
	void setVertexAttributePointers();
	void setVertexPositions();
	void setVertexNormals();
	void setTextureCoords();
	void setVertexTangent();
	void setVertexBitangent();
	void setIDs();
	void setWeights();
};

