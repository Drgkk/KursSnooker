#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Models.h"
#include "RigidBody.h"


class Sprite : public RigidBody
{
public:
	Sprite(glm::vec3 scale, std::shared_ptr<Model<>> model, ShaderProgram& shaderProgram);
	void Draw(float deltaTime, glm::mat4 proj, glm::mat4 view);
	ShaderProgram& GetShaderProgram();
private:
	std::shared_ptr<Model<>> model;
	ShaderProgram& shaderProgram;
	glm::vec3 scale;
};

