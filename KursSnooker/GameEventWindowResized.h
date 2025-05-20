#pragma once
#include "GameEvent.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "Window.h"
class GameEventWindowResized : public GameEvent
{
public:
	GameEventWindowResized(GLFWwindow* window, int width, int height);
	void Register() override;

private:
	GLFWwindow* window;
	int width;
	int height;

};

