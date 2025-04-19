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

	Map* GetMap() { return map; }
	TurnManager* GetTurnManager() { return turnManager; }

	// test. 스페이스바를 누르면 맵에 배치
	void SetEntityOnMap(FPOINT pos);

};

