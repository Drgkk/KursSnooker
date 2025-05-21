#pragma once
#include "Sprite.h"
#include "RigidBody.h"

class SnookerBall : public RigidBody
{
public:
	SnookerBall(std::unique_ptr<Sprite> sprite);
protected:
	void onContact(RigidBody& other) override;
private:
	std::unique_ptr<Sprite> sprite;
};

