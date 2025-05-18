#include "Sprite.h"

const float TERMINAL_VELOCITY = 3.0f;

Sprite::Sprite(SpriteConfig cfg)
	: pos(cfg.pos), rotation(cfg.rotation), scale(cfg.scale), model(cfg.model), g(cfg.g), velocity(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

Sprite::Sprite(const Sprite& other)
	: pos(other.pos), rotation(other.rotation), scale(other.scale), model(other.model), g(other.g), velocity(other.velocity)
{

	for (int i = 0; i < other.boundingVolumes.size(); i++) {
		this->boundingVolumes.push_back(other.boundingVolumes[i]->clone());
	}

}

void Sprite::Draw(ShaderProgram& shaderProgram, float deltaTime, glm::mat4 proj, glm::mat4 view)
{
	if(abs(velocity.y) < TERMINAL_VELOCITY)
		velocity.y += -0.1 * g * deltaTime;

	pos += velocity * deltaTime;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::scale(modelMatrix, scale);
	modelMatrix = rotation * modelMatrix;
	shaderProgram.setMat4("model", modelMatrix);
	model.get()->Draw(shaderProgram);

	for (int i = 0; i < boundingVolumes.size(); i++) {
		boundingVolumes[i]->UpdatePos(pos);
		boundingVolumes[i]->UpdateRotation(rotation);
		boundingVolumes[i]->UpdateScale(scale);
		boundingVolumes[i]->Draw(proj, view);
	}
	shaderProgram.Use();
}

void Sprite::AddBoundingVolume(std::unique_ptr<CollisionBoundingVolume> cbv)
{
	this->boundingVolumes.push_back(std::move(cbv));
}


glm::vec3 Sprite::GetPosition()
{
	return pos;
}
