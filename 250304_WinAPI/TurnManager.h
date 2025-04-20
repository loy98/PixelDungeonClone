#pragma once
#include "config.h"

class Level;
class Entity;
struct PQActor {
	double time;      // 다음 액션 예정 시각 (초 단위)
	Entity* actor;

	bool operator>(PQActor const& other) const {
		return time > other.time;
	}
};

class TurnManager
{
private:
	vector<Entity*> turnQueue;
	int currentActorIndex = 0;

	// 에너지 턴 개념 test
	priority_queue<PQActor, vector<PQActor>, greater<PQActor>> schedule;
	float currentTime;
public:
	TurnManager();
	virtual ~TurnManager();

	void Init();

	void AddActor(Entity* actor);
	void ProcessTurns(Level* level);
	void EndTurn();
	Entity* GetCurrentActor();
};

