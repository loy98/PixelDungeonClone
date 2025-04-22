#include "Entity.h"
#include "Level.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "TimerManager.h"

Entity::Entity()
{
	image = D2DImageManager::GetInstance()->AddImage("entityTest", L"Image/warrior_walk_1.png", 8, 7);
	//image = D2DImageManager::GetInstance()->AddImage("entityTest", L"Image/rat.png", 16, 3);
	isActive = true;
}

Entity::~Entity()
{
}

void Entity::Update()
{
	curTime += TimerManager::GetInstance()->GetDeltaTime();
	if (curTime > 0.1f)
	{
		curAnimFrame++;
		curTime = 0;
		if (curAnimFrame >= 8)
			curAnimFrame = 2;
	}
}

void Entity::Render(HDC hdc)
{
	// Rectangle(hdc, position.x - TILE_SIZE / 2, position.y - TILE_SIZE / 2,
	// 	position.x + TILE_SIZE / 2, position.y + TILE_SIZE / 2);
	if (curState != EntityState::ATTACK)
		image->Middle_RenderFrameScale(position.x, position.y, 2, 2, curAnimFrame, 1 );
	//image->Middle_RenderFrame(position.x, position.y, 0, 0);
	//wsprintf(szText, TEXT("hp:%d, att:%d, df:%d"), hp, attackDmg, defense);
	//TextOut(hdc, position.x - TILE_SIZE, position.y, szText, wcslen(szText));
  
	//if (image)
  //      image->Middle_RenderFrameScale(position.x, position.y, 2.f, 2.f, 0, 0);
}

void Entity::Act(Level* level)
{
}

void Entity::TakeDamage(int dmg)
{
	hp -= dmg;
	Stop();
	if (hp <= 0)
	{
		hp = 0;
		isActive = false;
		curState = EntityState::DEAD;
	}
}

void Entity::TakeExp(int exp)
{
	this->exp += exp;
	if (this->exp / level == 20)
	{
		this->exp = this->exp % level;
		LevelUp();
	}
}

void Entity::LevelUp()
{
	level++;
	attackDmg *= 1.5;
	maxHp += 10;
	hp += 10;
}
