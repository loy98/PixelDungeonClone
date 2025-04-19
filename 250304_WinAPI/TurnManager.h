#pragma once
#include "config.h"

class Entity;
class Game;
class TurnManager
{
private:
	vector<Entity*> turnQueue;
	int currentActorIndex = 0;
public:
	TurnManager();
	virtual ~TurnManager();

	void AddActor(Entity* actor);
	void ProcessTurns(Game* game);
	void EndTurn();
	Entity* GetCurrentActor();
};

