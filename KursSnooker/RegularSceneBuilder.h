#pragma once
#include "SceneBuilder.h"
#include "Scene.h"
#include "ObjectAssimpParser.h"
#include "CollisionBoundingVolume.h"
#include "Box.h"
#include "Sphere.h"

class RegularSceneBuilder : public SceneBuilder
{
public:
	RegularSceneBuilder(ShaderProgram& spritesShader, ShaderProgram& lightsShader, glm::vec3 skyboxColor);
	void BuildSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation) const override;
	std::shared_ptr<Sprite> CreateSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation) const override;
	void AddBox(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, glm::vec3 halfSize, ShaderProgram& shaderProgram,
		float inverseMass, float linearDamping = 0.9f, float angularDamping = 0.9f, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 angularVelocity = glm::vec3(0.0f)) override;
	void AddSphere(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, float radius, ShaderProgram& shaderProgram,
		float inverseMass, float linearDamping = 0.9f, float angularDamping = 0.9f, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 angularVelocity = glm::vec3(0.0f)) override;
	void BuildLightSource(std::shared_ptr<Sprite> sprite, std::unique_ptr<LightSourceSettings> settings) const override;
	void Reset();
	std::unique_ptr<Scene> Build();
private:
	std::unique_ptr<Scene> scene;
	ShaderProgram& spritesShader;
	ShaderProgram& lightsShader;
	glm::vec3 skyboxColor;
};

