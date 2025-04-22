#include "Entity.h"
#include "Level.h"
#include "D2DImage.h"
#include "D2DImageManager.h"

Entity::Entity()
{
	image = D2DImageManager::GetInstance()->AddImage("entityTest", L"Image/rat.png", 16, 3);
	isActive = true;
}

Entity::~Entity()
{
}

void Entity::Render(HDC hdc)
{
	// Rectangle(hdc, position.x - TILE_SIZE / 2, position.y - TILE_SIZE / 2,
	// 	position.x + TILE_SIZE / 2, position.y + TILE_SIZE / 2);
	if (image)
        image->Middle_RenderFrameScale(position.x, position.y, 2.f, 2.f, 0, 0);
}

void Entity::Act(Level* level)
{
}
