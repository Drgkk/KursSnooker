#pragma once
#include <vector>
#include "Sprite.h"
#include "Camera.h"
#include "LightSource.h"
#include "Window.h"

struct SceneConfig {
	ShaderProgram& spritesShader;
	ShaderProgram& lightsShader;
	glm::vec3 skyboxColor;
};

class Scene
{
public:
	Scene(SceneConfig cfg);
	void AddSprite(Sprite& sprite);
	void AddLightSource(LightSource& sprite);
	void Draw(std::unique_ptr<Window> window);
private:
	std::vector<std::unique_ptr<Sprite>> sprites;
	Camera player;
	std::vector<std::unique_ptr<LightSource>> lightSources;
	ShaderProgram& spritesShader;
	ShaderProgram& lightsShader;
	glm::vec3 skyboxColor;

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

