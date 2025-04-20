#pragma once
#include "config.h"

class TurnManager;
class Entity;
class Map;
class Renderer;
class Game
{
private:
	TurnManager* turnManager;
	Map* map;
	Renderer* renderer;

	vector<Entity*> actors;
public:
	Game();
	virtual ~Game();

	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void AddActor(Entity* actor);
	void ProcessCombat(Entity* attacker, Entity* defender);

	Entity* GetActorAt(FPOINT pos);
	Map* GetMap() { return map; }
	TurnManager* GetTurnManager() { return turnManager; }
};

