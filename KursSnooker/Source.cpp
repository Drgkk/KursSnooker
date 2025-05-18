#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <filesystem>
#include "SnookerImport.h"
#include "stb_image.h"




const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;



int main() {
	std::unique_ptr<Window> window = std::make_unique<Window>(SCR_WIDTH, SCR_HEIGHT, "Snooker");
	GLADWrapper gladWrapper;
	gladWrapper.Initialize();
	GameEvents gameEvents;
	gameEvents.AddEvent(new GameEventWindowResized(window.get()));
	gameEvents.Register();

	
	glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	std::unique_ptr<ShadersBuilder> shadersBuilder = std::make_unique<ShadersBuilder>();
	shadersBuilder->BuildVertexShader("shaders/objectshader/objectshader.vs");
	shadersBuilder->BuildFragmentShader("shaders/objectshader/objectshader.fs");
	std::unique_ptr<Shaders> objectShaders = shadersBuilder->Build();
	shadersBuilder->BuildVertexShader("shaders/lightshader/lightshader.vs");
	shadersBuilder->BuildFragmentShader("shaders/lightshader/lightshader.fs");
	std::unique_ptr<Shaders> lightShaders = shadersBuilder->Build();
	shadersBuilder->BuildVertexShader("shaders/collisionvolumesshader/collisionvolumesshader.vs");
	shadersBuilder->BuildFragmentShader("shaders/collisionvolumesshader/collisionvolumesshader.fs");
	std::unique_ptr<Shaders> collisionVolumesShaders = shadersBuilder->Build();
	ShaderProgram objectShaderProgram(std::move(objectShaders));
	ShaderProgram lightShaderProgram(std::move(lightShaders));
	ShaderProgram collisionVolumesShaderProgram(std::move(collisionVolumesShaders));


	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

	std::unique_ptr<BoundingVolumesBuilder> collisionBuilder = std::make_unique<BoundingVolumesBuilder>();


	std::unique_ptr<RegularSceneBuilder> sceneBuilder = std::make_unique<RegularSceneBuilder>(SceneConfig{
		.spritesShader = objectShaderProgram,
		.lightsShader = lightShaderProgram,
		.skyboxColor = glm::vec3(0.05f, 0.05f, 0.05f)
		});
	collisionBuilder->BuildOBB(OBBConfig{
			.Pos = glm::vec3(0.0f, 3.0f, 0.0f),
			.AxisX = glm::vec3(1.0f, 0.0f, 0.0f),
			.AxisY = glm::vec3(0.0f, 1.0f, 0.0f),
			.AxisZ = glm::vec3(0.0f, 0.0f, 1.0f),
			.HalfSize = glm::vec3(1.0f, 0.4f, 1.0f),
			.shaderProgram = collisionVolumesShaderProgram
		});
	sceneBuilder->BuildSprite(SceneBuilder::SceneSpriteBuilderConfig{
		.path = "resources/objects/snookertable/snookertable.obj",
		.pos = glm::vec3(0.0f, 0.0f, 0.0f),
		.scale = glm::vec3(1.0f, 1.0f, 1.0f),
		.rotation = glm::mat4(1.0f),
		.g = 0.0f,
		.boundingVolumes = std::move(collisionBuilder->Build())
		});
	collisionBuilder->BuildOBB(OBBConfig{
			.Pos = glm::vec3(0.0f, 0.0f, 0.0f),
			.AxisX = glm::vec3(1.0f, 0.0f, 0.0f),
			.AxisY = glm::vec3(0.0f, 1.0f, 0.0f),
			.AxisZ = glm::vec3(0.0f, 0.0f, 1.0f),
			.HalfSize = glm::vec3(6.5f, 0.06f, 6.5f),
			.shaderProgram = collisionVolumesShaderProgram
		});
	sceneBuilder->BuildSprite(SceneBuilder::SceneSpriteBuilderConfig{
		.path = "resources/objects/Floor/floor.obj",
		.pos = glm::vec3(0.0f, -0.8f, 0.0f),
		.scale = glm::vec3(1.0f, 1.0f, 1.0f),
		.rotation = glm::mat4(1.0f),
		.g = 0.0f,
		.boundingVolumes = std::move(collisionBuilder->Build())
		});
	collisionBuilder->BuildOBB(OBBConfig{
			.Pos = glm::vec3(0.0f, 0.0f, 0.0f),
			.AxisX = glm::vec3(1.0f, 0.0f, 0.0f),
			.AxisY = glm::vec3(0.0f, 1.0f, 0.0f),
			.AxisZ = glm::vec3(0.0f, 0.0f, 1.0f),
			.HalfSize = glm::vec3(1.0f, 1.0f, 1.0f),
			.shaderProgram = collisionVolumesShaderProgram
		});
	sceneBuilder->BuildSprite(SceneBuilder::SceneSpriteBuilderConfig{
		.path = "resources/objects/snookerballs/white/ball.obj",
		.pos = glm::vec3(0.0f, 0.12f, 0.0f),
		.scale = glm::vec3(0.04f, 0.04f, 0.04f),
		.rotation = glm::mat4(1.0f),
		.g = 0.0f,
		.boundingVolumes = std::move(collisionBuilder->Build())
		});
	collisionBuilder->BuildOBB(OBBConfig{
			.Pos = glm::vec3(12.0f, 0.0f, 0.0f),
			.AxisX = glm::vec3(1.0f, 0.0f, 0.0f),
			.AxisY = glm::vec3(0.0f, 1.0f, 0.0f),
			.AxisZ = glm::vec3(0.0f, 0.0f, 1.0f),
			.HalfSize = glm::vec3(1.0f, 1.0f, 1.0f),
			.shaderProgram = collisionVolumesShaderProgram
		});
	SceneBuilder::SceneSpriteBuilderConfig sc{
			.path = "resources/objects/snookerballs/1/ball1.obj",
			.pos = glm::vec3(0.0f, 2.5f, 0.0f),
			.scale = glm::vec3(0.04f, 0.04f, 0.04f),
			.rotation = glm::mat4(1.0f),
			.g = 0.0f,
			.boundingVolumes = std::move(collisionBuilder->Build())
	};
	Sprite sprite = sceneBuilder->CreateSprite(sc);
	sceneBuilder->BuildLightSource(SceneBuilder::SceneLightSourceBuilderConfig{
		.sprite = sprite,
		.settings = std::move(std::make_unique<LightSourceSettingsPoint>(LightSourceSettingsPointConfig{
			.ambient = glm::vec3(0.05f, 0.05f, 0.05f),
			.diffuse = glm::vec3(0.9f, 0.9f, 0.9f),
			.specular = glm::vec3(1.0f, 1.0f, 1.0f),
			.constant = 1.0f,
			.linear = 0.09f,
			.quadratic = 0.032f,
			.index = 0,
}))
		});


	std::unique_ptr<Scene> scene1(sceneBuilder->Build());
	

	

	Game snookerGame(std::move(window), std::move(scene1));
	snookerGame.Loop();


	return 0;
}
