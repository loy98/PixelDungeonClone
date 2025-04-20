#include "TurnManager.h"
#include "Game.h"
#include "Entity.h"

TurnManager::TurnManager()
{
}

TurnManager::~TurnManager()
{
}

void TurnManager::AddActor(Entity* actor)
{
	// 추가하려는 Entity가 이미 container에 있다면 return
	auto it = find(turnQueue.begin(), turnQueue.end(), actor);
	if (it != turnQueue.end())
		return;

	turnQueue.push_back(actor);
}

void TurnManager::ProcessTurns(Game* game)
{
	if (turnQueue.empty()) return;
	Entity* actor = GetCurrentActor();
	if (!actor || !actor->isActive)
	{
		EndTurn();
		return;
	}
	// 이전 상태 저장
	EntityState prevState = actor->GetState();

	// 실제 행동 호출 (state가 MOVE나 ATTACK으로 바뀝니다)
	actor->Act(game);

	// 이전에는 바쁜 상태였다가, 지금 Idle로 돌아왔다면 턴 종료
	bool wasBusy = (prevState != EntityState::IDLE);
	bool nowIdle = (actor->GetState() == EntityState::IDLE);
	if (wasBusy && nowIdle) 
	{
		EndTurn();
	}
}

void TurnManager::EndTurn()
{
	if (turnQueue.empty()) return;

	currentActorIndex = (currentActorIndex + 1) % turnQueue.size();
}

Entity* TurnManager::GetCurrentActor()
{
	if (turnQueue.empty()) return nullptr;

	return turnQueue[currentActorIndex];
}
