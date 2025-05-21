#include "SnookerBall.h"

SnookerBall::SnookerBall(std::unique_ptr<Sprite> sprite)
	: sprite(std::move(sprite))
{

}

void SnookerBall::onContact(RigidBody& other)
{
}
