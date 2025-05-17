#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <filesystem>
#include "SnookerImport.h"
#include "stb_image.h"


const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"TexCoords = aTexCoords;\n"
"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main()\n"
"{\n"
"FragColor = texture(texture_diffuse1, TexCoords);\n"
"}\0";

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

	Shader* vertexShader = new ShaderVertex(vertexShaderSource);
	vertexShader->Compile();
	Shader* fragmentShader = new ShaderFragment(fragmentShaderSource);
	fragmentShader->Compile();

	Shaders programShaders;
	programShaders.AddShader(vertexShader);
	programShaders.AddShader(fragmentShader);
	ShaderProgram shaderProgram(programShaders);


	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

	std::unique_ptr<RegularSceneBuilder> sceneBuilder = std::make_unique<RegularSceneBuilder>(SceneConfig{
		.spritesShader = shaderProgram,
		.lightsShader = shaderProgram,
		.skyboxColor = glm::vec3(0.05f, 0.05f, 0.05f)
		});
	sceneBuilder->BuildSprite(SceneBuilder::SceneSpriteBuilderConfig{
		.path = "resources/objects/snookertable/snookertable.obj",
		.pos = glm::vec3(0.0f, 0.0f, 0.0f),
		.scale = glm::vec3(1.0f, 1.0f, 1.0f),
		.rotation = glm::mat4(1.0f),
		.g = 0.0f
		});
	sceneBuilder->BuildSprite(SceneBuilder::SceneSpriteBuilderConfig{
		.path = "resources/objects/Floor/floor.obj",
		.pos = glm::vec3(0.0f, -0.8f, 0.0f),
		.scale = glm::vec3(1.0f, 1.0f, 1.0f),
		.rotation = glm::mat4(1.0f),
		.g = 0.0f
		});
	sceneBuilder->BuildSprite(SceneBuilder::SceneSpriteBuilderConfig{
		.path = "resources/objects/snookerballs/7/ball7.obj",
		.pos = glm::vec3(0.0f, 12.12f, 0.0f),
		.scale = glm::vec3(0.04f, 0.04f, 0.04f),
		.rotation = glm::mat4(1.0f),
		.g = 9.8f
		});



	std::unique_ptr<Scene> scene1(sceneBuilder->Build());
	



	Game snookerGame(std::move(window), shaderProgram, std::move(scene1));
	snookerGame.Loop();


	return 0;
}
