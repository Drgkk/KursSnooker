#pragma once
#include <vector>
#include "Sprite.h"
#include "Camera.h"
#include "LightSource.h"
#include "Window.h"
#include "CollisionDetector.h"
#include "CollisionBoundingVolume.h"


class Scene
{
public:
	Scene(ShaderProgram& spritesShader, ShaderProgram& lightsShader, glm::vec3 skyboxColor, unsigned int maxContacts, unsigned int iterations);
	void AddSprite(std::shared_ptr<Sprite> sprite);
	void AddCollisionBoundingVolume(std::unique_ptr<CollisionBoundingVolume> collisionBoundingVolume);
	std::vector<std::shared_ptr<Sprite>>& GetSprites();
	void AddLightSource(LightSource& sprite);
	void Draw(std::unique_ptr<Window> window);
	unsigned int GenerateContacts();
	void UpdateObjects(float duration);
	void StartFrame();
private:
	std::vector<std::shared_ptr<Sprite>> sprites;
	Camera player;
	std::vector<std::unique_ptr<LightSource>> lightSources;
	ShaderProgram& spritesShader;
	ShaderProgram& lightsShader;
	glm::vec3 skyboxColor;
	bool calculateIterations;
	ContactResolver contactResolver;
	std::vector<Contact> contacts;
	std::vector<std::unique_ptr<CollisionBoundingVolume>> collisionBoundingVolumes;

	void debug(glm::mat4 projection, glm::mat4 view);
	void update(float duration);
	CollisionData cData;

	unsigned int maxContacts;

	ShaderProgram& getSpritesShader();
	ShaderProgram& getLightsShader();


	float lastX;
	float lastY;
	bool firstMouse;
	float deltaTime;

	void OnMouseMove(double xposIn, double yposIn);
	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

	std::unique_ptr<Window> processInput(std::unique_ptr<Window> window);
};

