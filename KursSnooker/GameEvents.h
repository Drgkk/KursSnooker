#pragma once
#include "GameEvent.h"
#include <vector>
class GameEvents
{
public:
	void AddEvent(GameEvent* event);
	void Register();
private:
	std::vector<GameEvent*> events;
};

