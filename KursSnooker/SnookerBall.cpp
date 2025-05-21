#include "SnookerBall.h"

SnookerBall::SnookerBall(glm::vec3 scale, std::shared_ptr<Model<>> model, ShaderProgram& shaderProgram)
	: Sprite(scale, model, shaderProgram)
{
}

void SnookerBall::onContact(RigidBody* other)
{
	if (auto b = dynamic_cast<SnookerHole*>(other)) {
		this->position = glm::vec3(0.0f, 0.315f, 5.0f);
		this->velocity = glm::vec3(0.0f);
		this->rotation = glm::vec3(0.0f);
		this->isClippable = true;
		this->ClearAccumulators();
	}
}
