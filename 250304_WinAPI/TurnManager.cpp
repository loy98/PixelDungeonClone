#include "TurnManager.h"
#include "Entity.h"

TurnManager::TurnManager() : currentTime(0.f)
{
}

TurnManager::~TurnManager()
{
}

void TurnManager::Init()
{
	while (!schedule.empty()) schedule.pop();
	

	// 모든 액터를 시뮬레이션 시간 0에서 시작
	for (auto actor : turnQueue) {
		if (actor && actor->isActive) {
			schedule.push({ 0.0, actor });
		}
	}
}

void TurnManager::AddActor(Entity* actor)
{
	// 추가하려는 Entity가 이미 container에 있다면 return
	auto it = find(turnQueue.begin(), turnQueue.end(), actor);
	if (it != turnQueue.end())
		return;

	turnQueue.push_back(actor);
}

void TurnManager::ProcessTurns(Level* level)
{
	//if (turnQueue.empty()) return;
	//Entity* actor = GetCurrentActor();
	//if (!actor || !actor->isActive)
	//{
	//	EndTurn();
	//	return;
	//}
	// m2_merge code
	//  if (actor->NeedsInput()) {
	//  	actor->Act(level);
	//  }
	//  else {
	//  	actor->Act(level);
	//  	if (!actor->IsBusy()) {
	//  		EndTurn();
	//  	}
	//  }


	//// 이전 상태 저장
	//EntityState prevState = actor->GetState();

	//// 실제 행동 호출 (state가 MOVE나 ATTACK으로 바뀝니다)
	//actor->Act(level);

	//// 이전에는 바쁜 상태였다가, 지금 Idle로 돌아왔다면 턴 종료
	//bool wasBusy = (prevState != EntityState::IDLE);
	//bool nowIdle = (actor->GetState() == EntityState::IDLE);
	//if (wasBusy && nowIdle) 
	//{
	//	EndTurn();
	//}
	// 
	// 에너지 test
	if (turnQueue.empty()) return;
	Entity* actor = GetCurrentActor();
	if (!actor || !actor->isActive) {
		EndTurn();
		return;
	}
	// 행동 전 상태 저장
	EntityState prevState = actor->GetState();

	// 1) 한 틱 분량 행동
	if (actor->CanAct())
	{
		actor->Act(level);
	}

	bool wasBusy = (prevState != EntityState::IDLE);
	bool nowIdle = (actor->GetState() == EntityState::IDLE);

	// 이전 틱에 행동중이었고, 현재 행동이 끝났으면
	if (wasBusy && nowIdle)
	{
		// 한 턴으로 인식해서 에너지를 소모
		actor->UseEnergy();
		// 활동 에너지가 부족하면 턴 종료
		if (!actor->CanAct())
		{
			EndTurn();
			return;
		}
		// 활동 에너지가 남았으면 턴 유지 후 반복
		return;
	}
}

void TurnManager::EndTurn()
{
	if (turnQueue.empty()) return;
	turnQueue[currentActorIndex]->AddEnergy();
	currentActorIndex = (currentActorIndex + 1) % turnQueue.size();
}

Entity* TurnManager::GetCurrentActor()
{
	if (turnQueue.empty()) return nullptr;

	return turnQueue[currentActorIndex];
}
