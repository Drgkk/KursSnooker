#include "SnookerTable.h"

SnookerTable::SnookerTable(glm::vec3 scale, std::shared_ptr<Model<>> model, ShaderProgram& shaderProgram)
	: Sprite(scale, model, shaderProgram)
{
}

void SnookerTable::onContact(RigidBody* other)
{
}
