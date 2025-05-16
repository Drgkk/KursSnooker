#pragma once
#include "Window.h"
#include "Models.h"
#include "Camera.h"

template<typename ObjectParser, typename Mesh>
class Game
{
public:
	Game(Window* w, Models<ObjectParser, Mesh>& models, ShaderProgram& shaderProgram, Camera& camera);
	void Loop();
private:
	Window* window;
	Models<ObjectParser, Mesh>& models;
	ShaderProgram& shaderProgram;
	Camera& camera;
	float deltaTime;
	void processInput();
};

template<typename ObjectParser, typename Mesh>
Game<ObjectParser, Mesh>::Game(Window* w, Models<ObjectParser, Mesh>& models, ShaderProgram& shaderProgram, Camera& camera)
	: models(models), shaderProgram(shaderProgram), camera(camera)
{
	this->window = w;
	deltaTime = 0.0f;
}

template<typename ObjectParser, typename Mesh>
inline void Game<ObjectParser, Mesh>::Loop()
{
	float lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		processInput();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shaderProgram.setMat4("projection", projection);
		shaderProgram.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		shaderProgram.setMat4("model", model);
		models.Draw(shaderProgram);


		glfwSwapBuffers(window->GetGLFWWindow());
		glfwPollEvents();
	}

	glfwTerminate();
}

template<typename ObjectParser, typename Mesh>
inline void Game<ObjectParser, Mesh>::processInput()
{
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window->GetGLFWWindow(), true);

	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

