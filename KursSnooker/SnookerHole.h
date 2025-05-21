#pragma once
#include "Sprite.h"

class SnookerHole : public Sprite
{
public:
	SnookerHole(glm::vec3 scale, std::shared_ptr<Model<>> model, ShaderProgram& shaderProgram);
protected:
	void onContact(RigidBody* other) override;
private:
};

