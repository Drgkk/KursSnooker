#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Models.h"
#include "CollisionBoundingVolume.h"
#include "OBB.h"
#include "Camera.h"

struct SpriteConfig {
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 rotation = glm::mat4(1.0f);
	std::shared_ptr<Model<>> model;
	float g;
};

class Sprite
{
public:
	Sprite(SpriteConfig cfg);
	Sprite(const Sprite& other);
	void Draw(ShaderProgram &shaderProgram, float deltaTime, glm::mat4 proj, glm::mat4 view);
	void AddBoundingVolume(std::unique_ptr<CollisionBoundingVolume> cbv);
	glm::vec3 GetPosition();
private:
	std::shared_ptr<Model<>> model;
	std::vector<std::unique_ptr<CollisionBoundingVolume>> boundingVolumes;
	glm::vec3 pos;
	glm::vec3 scale;
	glm::mat4 rotation;
	glm::vec3 velocity;
	float g;
};

