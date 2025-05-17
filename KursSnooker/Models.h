#pragma once
#include <vector>
#include "Model.h"

template<typename ObjectParser = ObjectAssimpParser<>,
	typename MeshT = Mesh>
class Models
{
public:
	void AddModel(Model<ObjectParser, MeshT> model);
	void Draw(ShaderProgram &shaderProgram);
private:
	std::vector<Model<ObjectParser, MeshT>> models;
};

template<typename ObjectParser, typename MeshT>
void Models<ObjectParser, MeshT>::AddModel(Model<ObjectParser, MeshT> model)
{
	models.push_back(model);
}

template<typename ObjectParser, typename MeshT>
inline void Models<ObjectParser, MeshT>::Draw(ShaderProgram &shaderProgram)
{
	for (unsigned int i = 0; i < models.size(); i++) {
		models[i].Draw(shaderProgram);
	}
}
