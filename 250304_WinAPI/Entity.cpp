#include "Entity.h"
#include "Level.h"
#include "D2DImage.h"
#include "D2DImageManager.h"

Entity::Entity()
{
	image = D2DImageManager::GetInstance()->AddImage("entityTest", L"Image/ufo.bmp", 10, 1);
	isActive = true;
}

Entity::~Entity()
{
}

void Entity::Render(HDC hdc)
{
	// Rectangle(hdc, position.x - TILE_SIZE / 2, position.y - TILE_SIZE / 2,
	// 	position.x + TILE_SIZE / 2, position.y + TILE_SIZE / 2);
	image->Middle_RenderFrame(position.x, position.y,0,0 );

	// wsprintf(szText, TEXT("hp:%d, att:%d, df:%d"), hp, attackDmg, defense);
	// TextOut(hdc, position.x - TILE_SIZE, position.y, szText, wcslen(szText));
}

void Entity::Act(Level* level)
{
}

void Entity::TakeDamage(int dmg)
{
	hp -= dmg;
	if (hp <= 0)
	{
		hp = 0;
		isActive = false;
		curState = EntityState::DEAD;
	}
}