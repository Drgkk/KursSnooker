#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "OBB.h"
#include "ShaderProgram.h"
#include "Vertices.h"
#include "Indices.h"

class Box : public OBB
{
public:
	Box(ShaderProgram& shaderProgram, glm::mat4 offset, glm::vec3 halfSize, std::shared_ptr<RigidBody> body);
	virtual void Draw(glm::mat4 proj, glm::mat4 view);
private:
	Vertices vertices;
	Indices indices; 
	ShaderProgram& shaderProgram;
	void setUpDraw();
	void setUpVertices();
	void setUpIndices();
	void setUpTriangleIndices(unsigned int i1, unsigned int i2, unsigned int i3);
	unsigned int VAO;
};

