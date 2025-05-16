#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Textures.h"

template<typename ObjectParser, typename Mesh>
class Model
{
public:
	Model(std::string const& path, ObjectParser& objectParser, bool gamma = false);
	Textures& GetTexturesLoaded();
	std::vector<Mesh>& GetMeshes();
	void Draw(ShaderProgram &shaderProgram);
private:
	Textures textures_loaded;
	ObjectParser objectParser;
	std::vector<Mesh> meshes;
	bool gammaCorrection;
};


template<typename ObjectParser, typename Mesh>
Model<ObjectParser, Mesh>::Model(std::string const& path, ObjectParser& objectParser, bool gamma) : gammaCorrection(gamma)
{
	this->objectParser = objectParser;
	this->objectParser.Parse(path, *this);
}

template<typename ObjectParser, typename Mesh>
Textures& Model<ObjectParser, Mesh>::GetTexturesLoaded()
{
	return this->textures_loaded;
}

template<typename ObjectParser, typename Mesh>
std::vector<Mesh>& Model<ObjectParser, Mesh>::GetMeshes()
{
	return this->meshes;
}

template<typename ObjectParser, typename Mesh>
void Model<ObjectParser, Mesh>::Draw(ShaderProgram& shaderProgram)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shaderProgram);
	}
}
