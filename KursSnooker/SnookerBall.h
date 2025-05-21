#pragma once
#include "Sprite.h"
#include "CueBall.h"
#include "SnookerHole.h"

class SnookerBall : public Sprite
{
public:
	SnookerBall(glm::vec3 scale, std::shared_ptr<Model<>> model, ShaderProgram& shaderProgram);
protected:
	void onContact(RigidBody* other) override;
private:
};

