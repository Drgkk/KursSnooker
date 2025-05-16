#pragma once
#include "Vertices.h"
#include "Indices.h"
#include "Textures.h"
#include "ShaderProgram.h"
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh
{
public:
	Mesh(Vertices vertices, Indices indices, Textures textures);
	void Draw(ShaderProgram &shaderProgram);
private:
	Vertices vertices;
	Indices indices;
	Textures textures;
	void setupMesh();
	unsigned int VAO;
};


