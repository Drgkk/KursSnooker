#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Textures.h"
#include "ObjectAssimpParser.h"
#include "Mesh.h"

template<typename ObjectParser = ObjectAssimpParser<>,
	typename MeshT = Mesh>
class Model
{
public:
	Model(std::string const& path, ObjectParser& objectParser = ObjectParser{}, bool gamma = false);
	Textures& GetTexturesLoaded();
	std::vector<MeshT>& GetMeshes();
	void Draw(ShaderProgram &shaderProgram);
private:
	Textures textures_loaded;
	ObjectParser objectParser;
	std::vector<MeshT> meshes;
	bool gammaCorrection;
};


template<typename ObjectParser, typename MeshT>
Model<ObjectParser, MeshT>::Model(std::string const& path, ObjectParser& objectParser, bool gamma) : gammaCorrection(gamma), objectParser(objectParser)
{
	this->objectParser.Parse(path, *this);
}

template<typename ObjectParser, typename MeshT>
Textures& Model<ObjectParser, MeshT>::GetTexturesLoaded()
{
	return this->textures_loaded;
}

template<typename ObjectParser, typename MeshT>
std::vector<MeshT>& Model<ObjectParser, MeshT>::GetMeshes()
{
	return this->meshes;
}

template<typename ObjectParser, typename MeshT>
void Model<ObjectParser, MeshT>::Draw(ShaderProgram& shaderProgram)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shaderProgram);
	}
}
