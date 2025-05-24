#pragma once
#include <vector>
#include "Sprite.h"
#include "Camera.h"
#include "LightSource.h"
#include "Window.h"
#include "CollisionDetector.h"
#include "CollisionBoundingVolume.h"
#include "ForceRegistry.h"


class Scene
{
public:
	Scene(glm::vec3 skyboxColor, unsigned int maxContacts, unsigned int iterations, std::unique_ptr<ForceRegistry> fr);
	void AddSprite(std::shared_ptr<Sprite> sprite);
	void AddCollisionBoundingVolume(std::unique_ptr<CollisionBoundingVolume> collisionBoundingVolume);
	std::vector<std::shared_ptr<Sprite>>& GetSprites();
	void AddLightSource(LightSource& sprite);
	void Draw(Window* window);
	ForceRegistry* GetForceRegistry();
	unsigned int GenerateContacts();
	void UpdateObjects(float duration);
	void StartFrame();
private:
	Window* window;
	std::vector<std::shared_ptr<Sprite>> sprites;
	Camera player;
	std::vector<std::unique_ptr<LightSource>> lightSources;
	glm::vec3 skyboxColor;
	bool calculateIterations;
	ContactResolver contactResolver;
	std::vector<Contact> contacts;
	std::vector<std::unique_ptr<CollisionBoundingVolume>> collisionBoundingVolumes;

	void debug(glm::mat4 projection, glm::mat4 view);
	void update(float duration);
	CollisionData cData;


	unsigned int maxContacts;
	std::unique_ptr<ForceRegistry> fr;

	bool isMouseCaptured = true;
	bool isPaused = true;
	bool nextFrame = false;
	bool isNextFrameAlready = false;

	bool isRayHit = false;
	float forceMult = 10.0f;


	float lastX;
	float lastY;
	bool firstMouse;
	float deltaTime;

	void rayIntersect();
	void OnMouseMove(double xposIn, double yposIn);
	void OnWindowResize(int width, int height);
	void OnMouseScroll(double xoffset, double yoffset);
	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	static void window_resize(GLFWwindow* window, int width, int height);
	static void mouse_scroll(GLFWwindow* window, double xoffset, double yoffset);

	void processInput();
};

