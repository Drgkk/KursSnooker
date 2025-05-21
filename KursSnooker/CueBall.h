#pragma once
#include "Sprite.h"
#include "SnookerBall.h"
#include "SnookerHole.h"

class CueBall : public Sprite
{
public:
	CueBall(glm::vec3 scale, std::shared_ptr<Model<>> model, ShaderProgram& shaderProgram);
protected:
	void onContact(RigidBody* other) override;
private:
};

