#include "Armor.h"
#include "D2DImageManager.h"

Armor::Armor(FPOINT position, string name)
{
	eqType = EquipType::ARMOR;
	this->position = position;
	this->name = name;
	this->imageIdX = 0;
	this->imageIdY = 0;
	if (name == "갑옷1") { bonusValue = 5; level = 1; }
	else if (name == "갑옷2") { bonusValue = 10; level = 2; }
	image = D2DImageManager::GetInstance()->FindImage(name);
}

Armor::~Armor()
{
}

void Armor::Render(HDC hdc)
{
}
