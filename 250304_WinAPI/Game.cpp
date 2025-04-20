#include "Game.h"

#include "Entity.h"
#include "Player.h"
#include "Monster.h"
#include "TurnManager.h"
#include "KeyManager.h"
#include "CombatSyetem.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
	KeyManager::GetInstance()->Init();
	turnManager = new TurnManager();

	// 시작 위치 테스트용 매직넘버
	Entity* player = new Player({ TILE_SIZE / 2, TILE_SIZE  / 2}, 100.f, 20, 3, 3);
	Entity* monster1 = new Monster({TILE_SIZE /2 * 15, TILE_SIZE / 2 * 15 }, 100.f, 30, 5, 1);
	Entity* monster2 = new Monster({TILE_SIZE /2 * 19, TILE_SIZE / 2 * 25 }, 100.f, 30, 5, 1);

	AddActor(player);
	AddActor(monster1);
	AddActor(monster2);

	for (auto actor : actors)
	{
		if (actor)
		turnManager->AddActor(actor);
	}
}

void Game::Release()
{
	KeyManager::GetInstance()->Release();
	for (auto actor : actors)
	{
		if (actor)
		{
			// actor->Release();
			delete actor;
			actor = nullptr;
		}
	}
}

void Game::Update()
{
	turnManager->ProcessTurns(this);
}

void Game::Render(HDC hdc)
{
	for (auto actor : actors)
	{
		actor->Render(hdc);
	}
}

void Game::AddActor(Entity* actor)
{
	// 추가하려는 Entity가 이미 container에 있다면 return
	auto it = find(actors.begin(), actors.end(), actor);
	if (it != actors.end())
		return;

	actors.push_back(actor);
}

void Game::ProcessCombat(Entity* attacker, Entity* defender)
{
	CombatSyetem::GetInstance()->ProcessAttack(attacker, defender);
}

Entity* Game::GetActorAt(FPOINT pos)
{
	if (actors.empty()) return nullptr;

	for (auto actor : actors)
	{
		if (actor && actor->GetPosition() == pos)
			return actor;
	}
	return nullptr;
}
