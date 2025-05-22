#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <filesystem>
#include "SnookerImport.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


int main() {
	std::unique_ptr<Window> window = std::make_unique<Window>(SCR_WIDTH, SCR_HEIGHT, "Snooker");
	GLADWrapper gladWrapper;
	gladWrapper.Initialize();
	GameEvents gameEvents;
	/*gameEvents.AddEvent(new GameEventWindowResized(window->GetGLFWWindow(), window->GetWidth(), window->GetHeight()));
	gameEvents.Register();*/

	
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

	const float ballLinearDamping = 0.8f;
	const float ballAngularDamping = 0.75f;
	const float ballsDisplacement = 0.0f;
	const float ballsSize = 0.04f;
	glm::mat4 ballsRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ballsRotation = glm::rotate(ballsRotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	std::shared_ptr<Gravity> gravity = std::make_shared<Gravity>(glm::vec3(0.0f, -1.0f, 0.0f));

	std::unique_ptr<RegularSceneBuilder> sceneBuilder = std::make_unique<RegularSceneBuilder>(
		glm::vec3(0.05f, 0.05f, 0.05f), 250, 500, std::move(std::make_unique<ForceRegistry>())
		);
	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookertable/snookertable.obj", glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f)), objectShaderProgram
		);
	sceneBuilder->BuildSnookerBall(
		"resources/objects/Floor/floor.obj", glm::vec3(0.0f, -0.61f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::mat4(1.0f), objectShaderProgram
		);
	
	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/9/ball9.obj", glm::vec3(0.0f, 0.315f, 0.4f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
		);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/12/ball12.obj", glm::vec3(0.047f, 0.315f, 0.465f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));


	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/7/ball7.obj", glm::vec3(-0.047f, 0.315f, 0.465f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));



	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/8/ball8.obj", glm::vec3(0.0f, 0.315f, 0.53f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));


	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/15/ball15.obj", glm::vec3(-0.098f, 0.315f, 0.53f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));



	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/1/ball1.obj", glm::vec3(0.098f, 0.315f, 0.53f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/10/ball10.obj", glm::vec3(-0.047f, 0.315f, 0.597f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));



	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/3/ball3.obj", glm::vec3(0.047f, 0.315f, 0.597f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));



	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/6/ball6.obj", glm::vec3(-0.145f, 0.315f, 0.597f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));



	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/14/ball14.obj", glm::vec3(0.145f, 0.315f, 0.597f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));


	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/13/ball13.obj", glm::vec3(0.0f, 0.315f, 0.664f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));



	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/2/ball2.obj", glm::vec3(-0.098f, 0.315f, 0.664f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));



	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/4/ball4.obj", glm::vec3(0.098f, 0.315f, 0.664f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));



	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/11/ball11.obj", glm::vec3(-0.192f, 0.315f, 0.664f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));



	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/5/ball5.obj", glm::vec3(0.192f, 0.315f, 0.664f + ballsDisplacement), glm::vec3(ballsSize), ballsRotation, objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));



	sceneBuilder->BuildCueBall(
		"resources/objects/snookerballs/white/ball.obj", glm::vec3(0.0f, 0.315f, 0.0f), glm::vec3(ballsSize), glm::mat4(1.0f), objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 0.142f, false, ballLinearDamping, ballAngularDamping, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	sceneBuilder->BuildSnookerHole(
		"", glm::vec3(0.69f, 0.315f, 1.27f), glm::vec3(0.06f), glm::mat4(1.0f), collisionVolumesShaderProgram
	);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 1.0f, true, ballLinearDamping, ballAngularDamping);

	sceneBuilder->BuildSnookerHole(
		"", glm::vec3(-0.69f, 0.315f, 1.27f), glm::vec3(0.06f), glm::mat4(1.0f), lightShaderProgram
	);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 1.0f, true, ballLinearDamping, ballAngularDamping);

	sceneBuilder->BuildSnookerHole(
		"", glm::vec3(-0.69f, 0.315f, -1.27f), glm::vec3(0.06f), glm::mat4(1.0f), lightShaderProgram
	);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 1.0f, true, ballLinearDamping, ballAngularDamping);

	sceneBuilder->BuildSnookerHole(
		"", glm::vec3(0.69f, 0.315f, -1.27f), glm::vec3(0.06f), glm::mat4(1.0f), lightShaderProgram
	);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 1.0f, true, ballLinearDamping, ballAngularDamping);

	sceneBuilder->BuildSnookerHole(
		"", glm::vec3(0.74f, 0.315f, 0.0f), glm::vec3(0.06f), glm::mat4(1.0f), lightShaderProgram
	);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 1.0f, true, ballLinearDamping, ballAngularDamping);

	sceneBuilder->BuildSnookerHole(
		"", glm::vec3(-0.74f, 0.315f, 0.0f), glm::vec3(0.06f), glm::mat4(1.0f), lightShaderProgram
	);

	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), ballsSize, collisionVolumesShaderProgram, 1.0f, true, ballLinearDamping, ballAngularDamping);

	sceneBuilder->BuildCueBall(
		"resources/objects/crate/crate.obj", glm::vec3(0.0f, 12.1f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f),
		glm::rotate(glm::mat4(1.0f), glm::radians(70.0f), glm::normalize(glm::vec3(3.0f, 0.0f, 1.0f))), objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f), collisionVolumesShaderProgram,
		3.0f, false, 0.9f, 0.9f, glm::vec3(0.0f), glm::vec3(2.0f, 5.0f, 1.0f));

	sceneBuilder->BuildCueBall(
		"resources/objects/crate/crate.obj", glm::vec3(0.0f, 5.1f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f),
		glm::rotate(glm::mat4(1.0f), glm::radians(70.0f), glm::normalize(glm::vec3(3.0f, 0.0f, 1.0f))), objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f), collisionVolumesShaderProgram,
		15.0f, false, 0.9f, 0.9f, glm::vec3(0.0f), glm::vec3(2.0f, 5.0f, 1.0f));

	sceneBuilder->BuildCueBall(
		"resources/objects/crate/crate.obj", glm::vec3(0.0f, 7.1f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f),
		glm::rotate(glm::mat4(1.0f), glm::radians(70.0f), glm::normalize(glm::vec3(3.0f, 0.0f, 1.0f))), objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f), collisionVolumesShaderProgram,
		1.0f, false, 0.9f, 0.9f, glm::vec3(0.0f), glm::vec3(2.0f, 5.0f, 1.0f));

	sceneBuilder->BuildCueBall(
		"resources/objects/crate/crate.obj", glm::vec3(0.0f, 2.1f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f),
		glm::rotate(glm::mat4(1.0f), glm::radians(70.0f), glm::normalize(glm::vec3(3.0f, 0.0f, 1.0f))), objectShaderProgram
	);
	sceneBuilder->AddForce(gravity);

	sceneBuilder->AddBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f), collisionVolumesShaderProgram,
		5.0f, false, 0.9f, 0.9f, glm::vec3(0.0f), glm::vec3(2.0f, 5.0f, 1.0f));


	sceneBuilder->BuildSnookerBall(
		"resources/objects/snookerballs/1/ball1.obj", glm::vec3(0.0f, 7.5f, 0.0f), glm::vec3(0.04f, 0.04f, 0.04f), glm::mat4(1.0f), lightShaderProgram
	);
	/*sceneBuilder->AddForce(gravity);
	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), 0.04f, collisionVolumesShaderProgram, 0.142f, false, 0.7f, 0.75f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));*/
	sceneBuilder->AddLightSource(
		std::move(std::make_unique<LightSourceSettingsPoint>(LightSourceSettingsPointConfig{
			.ambient = glm::vec3(0.05f, 0.05f, 0.05f),
			.diffuse = glm::vec3(0.9f, 0.9f, 0.9f),
			.specular = glm::vec3(1.0f, 1.0f, 1.0f),
			.constant = 1.0f,
			.linear = 0.09f,
			.quadratic = 0.032f,
			.index = 0,
	})));
	sceneBuilder->BuildCueBall(
		"resources/objects/snookerballs/1/ball1.obj", glm::vec3(0.0f, 2.33f, -1.0f), glm::vec3(0.04f, 0.04f, 0.04f), glm::mat4(1.0f), lightShaderProgram
	);
	//sceneBuilder->AddForce(gravity);
	sceneBuilder->AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), 0.04f, collisionVolumesShaderProgram, 0.142f, false, 0.7f, 0.75f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f, 4.0f, 1.0f));
	sceneBuilder->AddLightSource(
		std::move(std::make_unique<LightSourceSettingsPoint>(LightSourceSettingsPointConfig{
			.ambient = glm::vec3(0.05f, 0.05f, 0.05f),
			.diffuse = glm::vec3(0.9f, 0.9f, 0.9f),
			.specular = glm::vec3(1.0f, 1.0f, 1.0f),
			.constant = 1.0f,
			.linear = 0.09f,
			.quadratic = 0.032f,
			.index = 1,
			})));


	std::unique_ptr<Scene> scene1(sceneBuilder->Build());
	

	

	Game snookerGame(std::move(window), std::move(scene1));
	snookerGame.Loop();


	return 0;
}
