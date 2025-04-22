#include "Game.h"

#include "Map.h"
#include "Entity.h"
#include "Player.h"
#include "Monster.h"
#include "TurnManager.h"
#include "KeyManager.h"

#define ASTAR_TILE_SIZE		30
#define ASTAR_TILE_COUNT	20

Game::Game()
{
	srand(unsigned(time(NULL)));
}

Game::~Game()
{
}

void Game::Init()
{
	KeyManager::GetInstance()->Init();
	turnManager = new TurnManager();
	
	map = new Map;

	// 랜덤 위치 설정 테스트
	//FPOINT arr[11];
	//for (int i = 0; i < 11; i++)
	//{
	//	float x = (rand() % TILE_X);
	//	float y = (rand() % TILE_Y);
	//	
	//	arr[i] = { x, y };
	//}

	// 시작 위치 테스트용 매직넘버

	//Entity* player = new Player({ TILE_SIZE / 2, TILE_SIZE  / 2});
	//Entity* monster1 = new Monster({TILE_SIZE /2 * 5, TILE_SIZE / 2 * 5 });
	//Entity* monster2 = new Monster({TILE_SIZE /2 * 9, TILE_SIZE / 2 });

	//AddActor(player);
	//AddActor(monster);

	Entity* player = new Player({ TILE_SIZE / 2, TILE_SIZE  / 2}, 100.f);
	Entity* monster1 = new Monster({TILE_SIZE /2 * 5, TILE_SIZE / 2 * 5 }, 100.f);
	Entity* monster2 = new Monster({TILE_SIZE /2 * 9, TILE_SIZE / 2 }, 100.f);


	// 게임이 맵을 가지고 있으면 바로 위치 설정 가능
	AddActor(new Player
	({ (float)ASTAR_TILE_SIZE / 2 + ASTAR_TILE_SIZE * (rand() % ASTAR_TILE_COUNT) , (float)ASTAR_TILE_SIZE / 2 + ASTAR_TILE_SIZE * (rand() % ASTAR_TILE_COUNT) }));
	for (int i = 0; i < 10; i++)
	{
		AddActor(new Monster);
	}

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

void Game::SetEntityOnMap(FPOINT pos)
{
	// 테스트용
	static int index = 0;
	index = (index + 1) % actors.size() + 1;
	if (index >= actors.size())
		return;
	actors[index]->SetPosition(pos);
	
}
