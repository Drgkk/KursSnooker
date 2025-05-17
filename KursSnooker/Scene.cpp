#include "Scene.h"



Scene::Scene(SceneConfig cfg)
	: spritesShader(cfg.spritesShader), lightsShader(cfg.lightsShader), skyboxColor(cfg.skyboxColor), player(glm::vec3(0.0f, 0.0f, 0.3f))
{
	deltaTime = 0.0f;
}

void Scene::AddSprite(Sprite& sprite)
{
	this->sprites.push_back(std::make_unique<Sprite>(sprite));
}

void Scene::AddLightSource(LightSource& lightSource)
{
	this->lightSources.push_back(std::make_unique<LightSource>(lightSource));
}


void Scene::Draw(std::unique_ptr<Window> window)
{
	glfwSetWindowUserPointer(window->GetGLFWWindow(), this);
	glfwSetCursorPosCallback(window->GetGLFWWindow(), &Scene::mouse_callback);
	float lastX = window->GetWidth() / 2.0f;
	float lastY = window->GetHeight() / 2.0f;
	firstMouse = true;
	float lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
		float currentTime = static_cast<float>(glfwGetTime());
		this->deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		window = processInput(std::move(window));

		glClearColor(skyboxColor.x, skyboxColor.y, skyboxColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(player.Zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
		glm::mat4 view = player.GetViewMatrix();

		getSpritesShader().Use();
		getSpritesShader().setMat4("projection", projection);
		getSpritesShader().setMat4("view", view);
		for (int i = 0; i < sprites.size(); i++) {
			sprites[i]->Draw(getSpritesShader(), deltaTime);
		}

		getLightsShader().Use();
		getLightsShader().setMat4("projection", projection);
		getLightsShader().setMat4("view", view);
		for (int i = 0; i < lightSources.size(); i++) {
			lightSources[i]->Draw(getLightsShader());
		}

		glfwSwapBuffers(window->GetGLFWWindow());
		glfwPollEvents();
	}
	
}


ShaderProgram& Scene::getSpritesShader()
{
	return spritesShader;
}

ShaderProgram& Scene::getLightsShader()
{
	return lightsShader;
}

void Scene::OnMouseMove(double xposIn, double yposIn)
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
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	player.ProcessMouseMovement(xoffset, yoffset);
}

void Scene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	Scene* scene = static_cast<Scene*>(
		glfwGetWindowUserPointer(window)
		);
	if (scene)
		scene->OnMouseMove(xposIn, yposIn);
}

std::unique_ptr<Window> Scene::processInput(std::unique_ptr<Window> window)
{
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window->GetGLFWWindow(), true);

	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_W) == GLFW_PRESS)
		player.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_S) == GLFW_PRESS)
		player.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_A) == GLFW_PRESS)
		player.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_D) == GLFW_PRESS)
		player.ProcessKeyboard(RIGHT, deltaTime);

	return std::move(window);
}

