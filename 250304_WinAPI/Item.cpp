#include "Item.h"
#include "D2DImageManager.h"
#include "Level.h"

Item::Item()
{
	image = D2DImageManager::GetInstance()->AddImage("아이템", L"Image\\Items.png", 16, 32);
	//D2DImageManager::GetInstance()->RegisterFrame("체력포션", "아이템", { 0, 336, 16, 352 });
	D2DImageManager::GetInstance()->AddImage("검1", L"Image\\sword1.png", 1, 1);
	D2DImageManager::GetInstance()->AddImage("검2", L"Image\\sword2.png", 1, 1);
	D2DImageManager::GetInstance()->AddImage("갑옷1", L"Image\\armor1.png", 1, 1);
	D2DImageManager::GetInstance()->AddImage("갑옷2", L"Image\\armor2.png", 1, 1);
	D2DImageManager::GetInstance()->AddImage("방패", L"Image\\shield.png", 1, 1);
}

Item::~Item()
{
}

void Item::Use(Entity* user, Level* level)
{
}
