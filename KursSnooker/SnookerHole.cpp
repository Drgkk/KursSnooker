#include "SnookerHole.h"

SnookerHole::SnookerHole(glm::vec3 scale, std::shared_ptr<Model<>> model, ShaderProgram& shaderProgram)
	: Sprite(scale, model, shaderProgram)
{
}

void SnookerHole::onContact(RigidBody* other)
{
}
