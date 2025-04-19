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
	srand(unsigned(time(NULL)));

	KeyManager::GetInstance()->Init();
	turnManager = new TurnManager();

	// 랜덤 위치 설정 테스트
	FPOINT arr[11];
	for (int i = 0; i < 11; i++)
	{
		float x = (rand() % TILE_X);
		float y = (rand() % TILE_Y);
		
		arr[i] = { x, y };
	}

	Entity* player = new Player({ (TILE_SIZE / 2) + TILE_SIZE*arr[0].x, (TILE_SIZE / 2) + TILE_SIZE * arr[0].y });
	Entity* monster1 = new Monster({ (TILE_SIZE / 2) + TILE_SIZE * arr[1].x, (TILE_SIZE / 2) + TILE_SIZE * arr[1].y });
	Entity* monster2 = new Monster({ (TILE_SIZE / 2) + TILE_SIZE * arr[2].x, (TILE_SIZE / 2) + TILE_SIZE * arr[2].y });
	Entity* monster3 = new Monster({ (TILE_SIZE / 2) + TILE_SIZE * arr[3].x, (TILE_SIZE / 2) + TILE_SIZE * arr[3].y });
	Entity* monster4 = new Monster({ (TILE_SIZE / 2) + TILE_SIZE * arr[4].x, (TILE_SIZE / 2) + TILE_SIZE * arr[4].y });
	Entity* monster5 = new Monster({ (TILE_SIZE / 2) + TILE_SIZE * arr[5].x, (TILE_SIZE / 2) + TILE_SIZE * arr[5].y });
	Entity* monster6 = new Monster({ (TILE_SIZE / 2) + TILE_SIZE * arr[6].x, (TILE_SIZE / 2) + TILE_SIZE * arr[6].y });
	Entity* monster7 = new Monster({ (TILE_SIZE / 2) + TILE_SIZE * arr[7].x, (TILE_SIZE / 2) + TILE_SIZE * arr[7].y });
	Entity* monster8 = new Monster({ (TILE_SIZE / 2) + TILE_SIZE * arr[8].x, (TILE_SIZE / 2) + TILE_SIZE * arr[8].y });
	Entity* monster9 = new Monster({ (TILE_SIZE / 2) + TILE_SIZE * arr[9].x, (TILE_SIZE / 2) + TILE_SIZE * arr[9].y });
	Entity* monster10 = new Monster({ (TILE_SIZE / 2) + TILE_SIZE * arr[10].x, (TILE_SIZE / 2) + TILE_SIZE * arr[10].y });
	// 시작 위치 테스트용 매직넘버
	//Entity* player = new Player({ TILE_SIZE / 2, TILE_SIZE  / 2});
	//Entity* monster1 = new Monster({TILE_SIZE /2 * 5, TILE_SIZE / 2 * 5 });
	//Entity* monster2 = new Monster({TILE_SIZE /2 * 9, TILE_SIZE / 2 });

	AddActor(player);
	AddActor(monster1);
	AddActor(monster2);
	AddActor(monster3);
	AddActor(monster4);
	AddActor(monster5);
	AddActor(monster6);
	AddActor(monster7);
	AddActor(monster8);
	AddActor(monster9);
	AddActor(monster10);

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
