#include "Game.h"

Game::Game(std::unique_ptr<Window> w, std::unique_ptr<Scene> scene)
	: scene(std::move(scene)), window(std::move(w))
{
}

void Game::Loop()
{
	scene->Draw(this->window.get());
	

	glfwTerminate();
}

