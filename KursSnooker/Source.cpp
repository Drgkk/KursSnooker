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
Camera camera(glm::vec3(0.0f, 0.0f, 0.3f));

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main() {
	Window* window = new Window(SCR_WIDTH, SCR_HEIGHT, "Snooker");
	GLADWrapper gladWrapper;
	gladWrapper.Initialize();
	GameEvents gameEvents;
	gameEvents.AddEvent(new GameEventWindowResized(window));
	gameEvents.Register();

	glfwSetCursorPosCallback(window->GetGLFWWindow(), mouse_callback);
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
	
	ObjectAssimpParser<Mesh, ImageLoaderSTB> objectParser;
	Models<ObjectAssimpParser<Mesh, ImageLoaderSTB>, Mesh> models;
	Model<ObjectAssimpParser<Mesh, ImageLoaderSTB>, Mesh> myModel1("resources/objects/backpack/backpack.obj", objectParser);
	Model<ObjectAssimpParser<Mesh, ImageLoaderSTB>, Mesh> myModel2("resources/objects/cilynder/Cilynder.obj", objectParser);
	Model<ObjectAssimpParser<Mesh, ImageLoaderSTB>, Mesh> myModel3("resources/objects/snookertable/snookertable.obj", objectParser);
	models.AddModel(myModel1);
	models.AddModel(myModel2);
	models.AddModel(myModel3);

	Game snookerGame(window, models, shaderProgram, camera);
	snookerGame.Loop();


	return 0;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}