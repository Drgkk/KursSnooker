#include "RegularSceneBuilder.h"

RegularSceneBuilder::RegularSceneBuilder(ShaderProgram& spritesShader, ShaderProgram& lightsShader, glm::vec3 skyboxColor)
	: spritesShader(spritesShader), lightsShader(lightsShader), skyboxColor(skyboxColor)
{
	this->Reset();
}

void RegularSceneBuilder::BuildSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
	glm::mat4 rotation) const
{
	std::shared_ptr<Sprite> sprite = CreateSprite(path, pos, scale, rotation);
	this->scene.get()->AddSprite(sprite);
}

std::shared_ptr<Sprite> RegularSceneBuilder::CreateSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
	glm::mat4 rotation) const
{
	ObjectAssimpParser<> objectAssimpParser;

	
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(pos, scale, rotation, std::make_shared<Model<>>(path, objectAssimpParser));

	return sprite;
}

void RegularSceneBuilder::AddBox(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, glm::vec3 halfSize, ShaderProgram& shaderProgram)
{
	glm::mat4 offset = glm::mat4(1.0f);
	offset[0] = glm::vec4(axisX, 0.0f);
	offset[1] = glm::vec4(axisY, 0.0f);
	offset[2] = glm::vec4(axisZ, 0.0f);
	offset = glm::translate(offset, relativePos);
	std::unique_ptr<Box> box = std::make_unique<Box>(shaderProgram, offset, halfSize, this->scene->GetSprites().back());
	this->scene->AddCollisionBoundingVolume(std::move(box));
}

void RegularSceneBuilder::BuildLightSource(std::shared_ptr<Sprite> sprite, std::unique_ptr<LightSourceSettings> settings) const
{
	LightSource lightSource(sprite, std::move(settings));
	this->scene.get()->AddLightSource(lightSource);
}

void RegularSceneBuilder::Reset()
{
	this->scene = std::make_unique<Scene>(spritesShader, lightsShader, skyboxColor, 100, 100);
}

std::unique_ptr<Scene> RegularSceneBuilder::Build()
{
	std::unique_ptr<Scene> pointer = std::move(this->scene);
	this->Reset();
	return pointer;
}

