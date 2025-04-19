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
	KeyManager::GetInstance()->Init();
	turnManager = new TurnManager();

	// 시작 위치 테스트용 매직넘버
	Entity* player = new Player({ TILE_SIZE / 2, TILE_SIZE  / 2}, 100.f);
	Entity* monster1 = new Monster({TILE_SIZE /2 * 5, TILE_SIZE / 2 * 5 }, 100.f);
	Entity* monster2 = new Monster({TILE_SIZE /2 * 9, TILE_SIZE / 2 }, 100.f);

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
