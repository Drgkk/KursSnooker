#include "Sprite.h"

const float TERMINAL_VELOCITY = 3.0f;

Sprite::Sprite(glm::vec3 scale, std::shared_ptr<Model<>> model, ShaderProgram& shaderProgram)
	: scale(scale), model(model), shaderProgram(shaderProgram)
{

}


void Sprite::Draw(float deltaTime, glm::mat4 proj, glm::mat4 view)
{
	shaderProgram.Use();
	shaderProgram.setMat4("projection", proj);
	shaderProgram.setMat4("view", view);
	shaderProgram.setMat4("model", glm::scale(this->transformMatrix, this->scale));
	model.get()->Draw(shaderProgram);
}

ShaderProgram& Sprite::GetShaderProgram()
{
	return shaderProgram;
}
