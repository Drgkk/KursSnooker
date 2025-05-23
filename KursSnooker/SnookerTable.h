#pragma once
#include "Sprite.h"


class SnookerTable : public Sprite
{
public:
	SnookerTable(glm::vec3 scale, std::shared_ptr<Model<>> model, ShaderProgram& shaderProgram);
protected:
	void onContact(RigidBody* other) override;
private:
};

