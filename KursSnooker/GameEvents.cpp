#include "GameEvents.h"

void GameEvents::AddEvent(GameEvent* event)
{
	events.push_back(event);
}

void GameEvents::Register()
{
	for (int i = 0; i < events.size(); i++) {
		events[i]->Register();
	}
}
