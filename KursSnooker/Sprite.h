#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Models.h"


class Sprite : public RigidBody
{
public:
	Sprite(glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, std::shared_ptr<Model<>> model);
	//Sprite(const Sprite& other);
	void Draw(ShaderProgram &shaderProgram, float deltaTime, glm::mat4 proj, glm::mat4 view);
	//void AddBoundingVolume(std::unique_ptr<CollisionBoundingVolume> cbv);
	//glm::vec3 GetPosition();
	//bool CheckCollision(const Sprite& other);
private:
	std::shared_ptr<Model<>> model;
	////std::vector<std::unique_ptr<CollisionBoundingVolume>> boundingVolumes;
	//glm::vec3 pos;
	glm::vec3 scale;
	//glm::mat4 rotation;
	//glm::vec3 velocity;
	//float g;
};

