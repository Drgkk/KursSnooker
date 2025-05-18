#pragma once
#include "Window.h"
#include "Models.h"
#include "Camera.h"
#include "Scene.h"

class Game
{
public:
	Game(std::unique_ptr<Window> w, std::unique_ptr<Scene> scene);
	void Loop();
private:
	std::unique_ptr<Window> window;
	std::unique_ptr<Scene> scene;
};

