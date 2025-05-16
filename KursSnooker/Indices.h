#pragma once
#include <vector>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Indices
{
public:
	void AddIndex(unsigned int i);
	unsigned int Size();
	void SetupEBO();
private:
	std::vector<unsigned int> indices;
	unsigned int EBO;
};

