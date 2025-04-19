#include "Entity.h"

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
}

void Entity::Act(Game* game)
{
}
