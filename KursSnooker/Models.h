#pragma once
#include <vector>
#include "Model.h"

template<typename ObjectParser, typename Mesh>
class Models
{
public:
	void AddModel(Model<ObjectParser, Mesh> model);
	void Draw(ShaderProgram &shaderProgram);
private:
	std::vector<Model<ObjectParser, Mesh>> models;
};

template<typename ObjectParser, typename Mesh>
void Models<ObjectParser, Mesh>::AddModel(Model<ObjectParser, Mesh> model)
{
	models.push_back(model);
}

template<typename ObjectParser, typename Mesh>
inline void Models<ObjectParser, Mesh>::Draw(ShaderProgram &shaderProgram)
{
	for (unsigned int i = 0; i < models.size(); i++) {
		models[i].Draw(shaderProgram);
	}
}
