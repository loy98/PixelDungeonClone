#include "Game.h"

#include "Entity.h"
#include "Player.h"
#include "Monster.h"
#include "TurnManager.h"
#include "KeyManager.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
	Entity* player = new Player({8, 8});
	Entity* monster = new Monster({24, 24});
	KeyManager::GetInstance()->Init();

	AddActor(player);
	AddActor(monster);

	for (auto actor : actors)
		turnManager->AddActor(actor);
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

void Game::AddActor(Entity* actor)
{
	// 추가하려는 Entity가 이미 container에 있다면 return
	auto it = find(actors.begin(), actors.end(), actor);
	if (it != actors.end())
		return;

	actors.push_back(actor);
}
