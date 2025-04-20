#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::Render(HDC hdc)
{
	Rectangle(hdc, position.x - TILE_SIZE / 2, position.y - TILE_SIZE / 2,
		position.x + TILE_SIZE / 2, position.y + TILE_SIZE / 2);

	wsprintf(szText, TEXT("hp:%d, att:%d, df:%d"), hp, attackDmg, defense);
	TextOut(hdc, position.x - TILE_SIZE, position.y, szText, wcslen(szText));
}

void Entity::Act(Game* game)
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
