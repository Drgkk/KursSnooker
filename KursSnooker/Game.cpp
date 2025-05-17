#include "Game.h"

Game::Game(std::unique_ptr<Window> w, ShaderProgram& shaderProgram, std::unique_ptr<Scene> scene)
	: shaderProgram(shaderProgram), scene(std::move(scene)), window(std::move(w))
{
}

void Game::Loop()
{
	scene->Draw(std::move(this->window));
	

	glfwTerminate();
}

