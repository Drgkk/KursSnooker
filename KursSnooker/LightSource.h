#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LightSourceSettings.h"
#include "ShaderProgram.h"
#include "ObjectAssimpParser.h"
#include "Sprite.h"

struct LightSourceConfig {
	Sprite sprite;
	std::unique_ptr<LightSourceSettings> settings;
};

class LightSource
{
public:
	LightSource(LightSourceConfig cfg);
	LightSource(const LightSource &other);
	void Draw(ShaderProgram &shaderProgram, float deltaTime, glm::mat4 proj, glm::mat4 view);
	void ApplyParameters(ShaderProgram& shaderProgram);
private:
	Sprite lightSprite;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::unique_ptr<LightSourceSettings> settings;
};

