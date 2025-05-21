#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sprite.h"
#include "LightSourceSettings.h"
#include "CollisionBoundingVolume.h"
#include "OBB.h"
#include "ForceGenerator.h"
#include "ForceRegistry.h"

class SceneBuilder
{
public:
	virtual ~SceneBuilder(){}
	/*virtual void BuildSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const = 0;*/
	virtual void BuildSnookerBall(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const = 0;
	virtual void BuildCueBall(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const = 0;
	virtual void BuildSnookerHole(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const = 0;
	
	virtual void AddBox(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, glm::vec3 halfSize, ShaderProgram& shaderProgram,
		float mass, bool isClipable, float linearDamping = 0.9f, float angularDamping = 0.9f, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 angularVelocity = glm::vec3(0.0f)) = 0;
	virtual void AddSphere(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, float radius, ShaderProgram& shaderProgram,
		float mass, bool isClipable, float linearDamping = 0.9f, float angularDamping = 0.9f, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 angularVelocity = glm::vec3(0.0f)) = 0;
	virtual void AddForce(std::shared_ptr<ForceGenerator> fg) = 0;
	virtual void AddLightSource(std::unique_ptr<LightSourceSettings> settings) const = 0;
private:
	virtual std::shared_ptr<Sprite> createSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
		glm::mat4 rotation, ShaderProgram& shaderProgram) const = 0;
};

