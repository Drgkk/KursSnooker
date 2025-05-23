#pragma once
#include "SceneBuilder.h"
#include "Scene.h"
#include "ObjectAssimpParser.h"
#include "CollisionBoundingVolume.h"
#include "Box.h"
#include "Sphere.h"
#include "SnookerBall.h"
#include "CueBall.h"
#include "SnookerHole.h"


class RegularSceneBuilder : public SceneBuilder
{
public:
	RegularSceneBuilder(glm::vec3 skyboxColor,
		unsigned int maxContacts, unsigned int iterations, std::unique_ptr<ForceRegistry> fr);
	/*void BuildSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const override;*/
	void BuildSnookerBall(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const override;
	void BuildCueBall(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const override;
	void BuildSnookerHole(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const override;
	void BuildSnookerTable(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const;
	
	void AddBox(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, glm::vec3 halfSize, ShaderProgram& shaderProgram,
		float mass, bool isClipable = false, float linearDamping = 0.9f, float angularDamping = 0.9f, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 angularVelocity = glm::vec3(0.0f)) override;
	void AddSphere(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, float radius, ShaderProgram& shaderProgram,
		float mass, bool isClipable = false, float linearDamping = 0.9f, float angularDamping = 0.9f, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 angularVelocity = glm::vec3(0.0f)) override;
	void AddForce(std::shared_ptr<ForceGenerator> fg) override;
	void AddLightSource(std::unique_ptr<LightSourceSettings> settings) const override;
	void Reset();
	std::unique_ptr<Scene> Build();
private:
	std::unique_ptr<Scene> scene;
	ShaderProgram& spritesShader;
	ShaderProgram& lightsShader;
	glm::vec3 skyboxColor;
	unsigned int maxContacts;
	unsigned int iterations;
	std::unique_ptr<ForceRegistry> fr;

	std::shared_ptr<Sprite> createSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const override;
};

