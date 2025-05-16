#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Textures
{
public:
	void Bind(ShaderProgram& shaderProgram);
	void InsertTexture(Textures tex);
	void AddTexture(Texture tex);
	std::vector<Texture>::iterator Begin();
	std::vector<Texture>::iterator End();
private:
	std::vector<Texture> textures;
};

