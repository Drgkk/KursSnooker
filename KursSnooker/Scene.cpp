#include "Scene.h"



Scene::Scene(glm::vec3 skyboxColor,
	 unsigned int maxContacts, unsigned int iterations, std::unique_ptr<ForceRegistry> fr)
	: skyboxColor(skyboxColor), player(glm::vec3(0.0f, 0.5f, 0.0f)),
     contactResolver(iterations), maxContacts(maxContacts), fr(std::move(fr))
{
	this->contactResolver.SetEpsilon(0.01f, 0.0005f);
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


void Scene::Draw(Window* window)
{
	this->window = window;
	glfwSetWindowUserPointer(window->GetGLFWWindow(), this);
	glfwSetCursorPosCallback(window->GetGLFWWindow(), &Scene::mouse_callback);
	glfwSetFramebufferSizeCallback(window->GetGLFWWindow(), &Scene::window_resize);
	glfwSetScrollCallback(window->GetGLFWWindow(), &Scene::mouse_scroll);
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
		processInput();
		if (!isPaused || (nextFrame && !isNextFrameAlready)) {
			update(deltaTime);
			nextFrame = false;
			isNextFrameAlready = true;
		}
			


		glClearColor(skyboxColor.x, skyboxColor.y, skyboxColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(player.Zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
		glm::mat4 view = player.GetViewMatrix();

		//debug(projection, view);

		
		for (int i = 0; i < sprites.size(); i++) {
			for (int j = 0; j < lightSources.size(); j++) {
				lightSources[j]->ApplyParameters(sprites[i]->GetShaderProgram());
			}
			sprites[i]->Draw(deltaTime, projection, view);
		}

		for (int i = 0; i < lightSources.size(); i++) {
			lightSources[i]->Draw(deltaTime, projection, view);
		}

		

		glfwSwapBuffers(window->GetGLFWWindow());
		glfwPollEvents();
	}
	
}

ForceRegistry* Scene::GetForceRegistry()
{
	return this->fr.get();
}

unsigned int Scene::GenerateContacts()
{
	unsigned int limit = maxContacts;

	CollisionPlane plane;
	plane.direction = glm::vec3(0.0f, 1.0f, 0.0f);
	plane.offset = 0.279f;

	CollisionPlane wall1;
	wall1.direction = glm::vec3(1.0f, 0.0f, 0.0f);
	wall1.offset = -0.7f;
	CollisionPlane wall2;
	wall2.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	wall2.offset = -0.7f;
	CollisionPlane wall3;
	wall3.direction = glm::vec3(0.0f, 0.0f, 1.0f);
	wall3.offset = -1.242f;
	CollisionPlane wall4;
	wall4.direction = glm::vec3(0.0f, 0.0f, -1.0f);
	wall4.offset = -1.242f;

	cData.reset(maxContacts);
	cData.friction = 0.05f;
	cData.restitution = 0.6f;
	cData.tolerance = 0.1f;

	for (int i = 0; i < collisionBoundingVolumes.size(); i++) {
		if (!cData.hasMoreContacts()) return 0;
		collisionBoundingVolumes[i]->IntersectsHalfSpace(plane, &cData);
		if (!cData.hasMoreContacts()) return 0;
		collisionBoundingVolumes[i]->IntersectsHalfSpace(wall1, &cData);
		if (!cData.hasMoreContacts()) return 0;
		collisionBoundingVolumes[i]->IntersectsHalfSpace(wall2, &cData);
		if (!cData.hasMoreContacts()) return 0;
		collisionBoundingVolumes[i]->IntersectsHalfSpace(wall3, &cData);
		if (!cData.hasMoreContacts()) return 0;
		collisionBoundingVolumes[i]->IntersectsHalfSpace(wall4, &cData);
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
	fr->UpdateForces(duration);
	UpdateObjects(duration);
	GenerateContacts();
	contactResolver.ResolveContacts(
		cData.contacts,
		cData.contactCount,
		duration
	);
}


void Scene::rayIntersect()
{
	glm::mat4 proj = glm::perspective(glm::radians(player.Zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
	glm::mat4 view = player.GetViewMatrix();

	glm::vec3 nearPt = glm::unProject(
		glm::vec3(lastX, (float)window->GetHeight() - lastY, 0.0f),
		view, proj, glm::vec4(0.0f, 0.0f, (float)window->GetWidth(), (float)window->GetHeight())
	);
	glm::vec3 farPt = glm::unProject(
		glm::vec3(lastX, (float)window->GetHeight() - lastY, 1.0f),
		view, proj, glm::vec4(0.0f, 0.0f, (float)window->GetWidth(), (float)window->GetHeight())
	);


	glm::vec3 ray = glm::normalize(farPt - nearPt);

	

	unsigned int bestIndex;
	glm::vec3 closestPoint(std::numeric_limits<float>::max());
	glm::vec3 worldPoint(std::numeric_limits<float>::max());

	for (int i = 0; i < collisionBoundingVolumes.size(); i++) {
		collisionBoundingVolumes[i]->IntersectsRay(nearPt, farPt, worldPoint);
		if (worldPoint.x != std::numeric_limits<float>::max()) {
			if (closestPoint.x == std::numeric_limits<float>::max()) {
				closestPoint = worldPoint;
				bestIndex = i;
			}
			else {
				if (glm::length(worldPoint - nearPt) < glm::length(closestPoint - nearPt)) {
					closestPoint = worldPoint;
					bestIndex = i;
				}
			}
		}
	}



	if (closestPoint.x != std::numeric_limits<float>::max()) {
		collisionBoundingVolumes[bestIndex]->body->AddForceAtPoint(forceMult * ray, closestPoint);
	}

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

	if(isMouseCaptured)
		player.ProcessMouseMovement(xoffset, yoffset);
}

void Scene::OnWindowResize(int width, int height)
{
	this->window->SetWidth(width);
	this->window->SetHeight(height);
}

void Scene::OnMouseScroll(double xoffset, double yoffset)
{
	forceMult += 2.0f*(float)yoffset;
	if (forceMult < 10.0f)
		forceMult = 10.0f;
	if (forceMult > 170.0f)
		forceMult = 170.0f;

}

void Scene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	Scene* scene = static_cast<Scene*>(
		glfwGetWindowUserPointer(window)
		);
	if (scene)
		scene->OnMouseMove(xposIn, yposIn);
}

void Scene::window_resize(GLFWwindow* window, int width, int height)
{
	Scene* scene = static_cast<Scene*>(
		glfwGetWindowUserPointer(window)
		);
	glViewport(0, 0, width, height);
	if (scene)
		scene->OnWindowResize(width, height);
}

void Scene::mouse_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	Scene* scene = static_cast<Scene*>(
		glfwGetWindowUserPointer(window)
		);
	if (scene)
		scene->OnMouseScroll(xoffset, yoffset);
}

void Scene::processInput()
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
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_R) == GLFW_PRESS)
		player.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_F) == GLFW_PRESS)
		player.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_P) == GLFW_PRESS)
		this->isPaused = true;
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_G) == GLFW_PRESS)
		this->isPaused = false;
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_J) == GLFW_PRESS) {
		this->nextFrame = true;
	}
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_J) == GLFW_RELEASE) {
		this->nextFrame = false;
		this->isNextFrameAlready = false;
	}
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_Y) == GLFW_PRESS) {
		glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		isMouseCaptured = true;
	}
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_N) == GLFW_PRESS) {
		glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		isMouseCaptured = false;
	}
	
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_T) == GLFW_PRESS && !isRayHit) {
		isRayHit = true;
		this->rayIntersect();
	}
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_T) == GLFW_RELEASE) {
		isRayHit = false;
	}

}

