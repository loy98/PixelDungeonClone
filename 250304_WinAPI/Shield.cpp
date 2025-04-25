#include "Shield.h"
#include "D2DImageManager.h"

Shield::Shield(FPOINT position, string name)
{
	eqType = EquipType::SHIELD;
	this->position = position;
	this->name = name;
	this->imageIdX = 0;
	this->imageIdY = 0;
	bonusValue = 1; 
	level = 1;
	image = D2DImageManager::GetInstance()->FindImage(name);
}

Shield::~Shield()
{
}

void Shield::Render(HDC hdc)
{
}
