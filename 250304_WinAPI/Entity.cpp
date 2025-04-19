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
}

void Entity::Act(Game* game)
{
}
