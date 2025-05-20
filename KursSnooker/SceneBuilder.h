#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sprite.h"
#include "LightSourceSettings.h"
#include "CollisionBoundingVolume.h"
#include "OBB.h"

class SceneBuilder
{
public:
	virtual ~SceneBuilder(){}
	virtual void BuildSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation) const = 0;
	virtual std::shared_ptr<Sprite> CreateSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation) const = 0;
	virtual void AddBox(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, glm::vec3 halfSize, ShaderProgram& shaderProgram,
		float inverseMass, float linearDamping = 0.9f, float angularDamping = 0.9f, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 angularVelocity = glm::vec3(0.0f)) = 0;
	virtual void AddSphere(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, float radius, ShaderProgram& shaderProgram,
		float inverseMass, float linearDamping = 0.9f, float angularDamping = 0.9f, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 angularVelocity = glm::vec3(0.0f)) = 0;
	virtual void BuildLightSource(std::shared_ptr<Sprite> sprite, std::unique_ptr<LightSourceSettings> settings) const = 0;
};

