#include "Scene.h"



Scene::Scene(ShaderProgram& spritesShader, ShaderProgram& lightsShader, glm::vec3 skyboxColor,
	 unsigned int maxContacts, unsigned int iterations)
	: spritesShader(spritesShader), lightsShader(lightsShader), skyboxColor(skyboxColor), player(glm::vec3(0.0f, 0.0f, 0.3f)),
     contactResolver(maxContacts), maxContacts(maxContacts)
{
	deltaTime = 0.0f;
	calculateIterations = (iterations == 0);
}

void Scene::AddSprite(std::shared_ptr<Sprite> sprite)
{
	this->sprites.push_back(sprite);
}

void Scene::AddCollisionBoundingVolume(std::unique_ptr<CollisionBoundingVolume> collisionBoundingVolume)
{
	this->collisionBoundingVolumes.push_back(std::move(collisionBoundingVolume));
}

std::vector<std::shared_ptr<Sprite>>& Scene::GetSprites()
{
	return this->sprites;
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
	int count = 1;
	while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
		float currentTime = static_cast<float>(glfwGetTime());
		this->deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		StartFrame();
		update(deltaTime);
		//collisionDetector->Resolve(sprites);

		window = processInput(std::move(window));

		glClearColor(skyboxColor.x, skyboxColor.y, skyboxColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(player.Zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
		glm::mat4 view = player.GetViewMatrix();

		debug(projection, view);

		getSpritesShader().Use();
		getSpritesShader().setMat4("projection", projection);
		getSpritesShader().setMat4("view", view);
		for (int i = 0; i < lightSources.size(); i++) {
			lightSources[i]->ApplyParameters(getSpritesShader());
		}
		for (int i = 0; i < sprites.size(); i++) {
			sprites[i]->Draw(getSpritesShader(), deltaTime, projection, view);
		}

		getLightsShader().Use();
		getLightsShader().setMat4("projection", projection);
		getLightsShader().setMat4("view", view);
		for (int i = 0; i < lightSources.size(); i++) {
			lightSources[i]->Draw(getLightsShader(), deltaTime, projection, view);
		}

		glfwSwapBuffers(window->GetGLFWWindow());
		glfwPollEvents();
	}
	
}

unsigned int Scene::GenerateContacts()
{
	unsigned int limit = maxContacts;

	CollisionPlane plane;
	plane.direction = glm::vec3(0.0f, 1.0f, 0.0f);
	plane.offset = -1.7f;

	cData.reset(maxContacts);
	cData.friction = 0.9f;
	cData.restitution = 0.1f;
	cData.tolerance = 0.1f;

	for (int i = 0; i < collisionBoundingVolumes.size(); i++) {
		if (!cData.hasMoreContacts()) return 0;
		collisionBoundingVolumes[i]->IntersectsHalfSpace(plane, &cData);
	}

	for (int i = 0; i < collisionBoundingVolumes.size(); i++) {
		for (int j = i + 1; j < collisionBoundingVolumes.size(); j++) {
			if (!cData.hasMoreContacts()) return 0;
			collisionBoundingVolumes[i]->Intersects(*collisionBoundingVolumes[j].get(), &cData);
		}
	}
	return 1;
}

void Scene::UpdateObjects(float duration)
{
	for (int i = 0; i < collisionBoundingVolumes.size(); i++) {
		collisionBoundingVolumes[i]->body->Integrate(duration);
		collisionBoundingVolumes[i]->CalculateInternals();
	}
}

void Scene::StartFrame()
{
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i]->ClearAccumulators();
		sprites[i]->CalculateDerivedData();
	}
}


void Scene::debug(glm::mat4 projection, glm::mat4 view)
{
	for (int i = 0; i < collisionBoundingVolumes.size(); i++) {
		collisionBoundingVolumes[i]->Draw(projection, view);
	}
}

void Scene::update(float duration)
{
	UpdateObjects(duration);
	GenerateContacts();
	contactResolver.ResolveContacts(
		cData.contacts,
		cData.contactCount,
		duration
	);
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

