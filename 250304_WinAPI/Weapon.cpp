#include "Weapon.h"
#include "D2DImageManager.h"

Weapon::Weapon(FPOINT position, string name)
{
	eqType = EquipType::WEAPON;
	this->position = position;
	this->name = name;
	this->imageIdX = 0;
	this->imageIdY = 0;
	if (name == "검1")	bonusValue = 1;
	else if (name == "검2") bonusValue = 2;
	image = D2DImageManager::GetInstance()->FindImage(name);
}

Weapon::~Weapon()
{
}

void Weapon::Render(HDC hdc)
{
}
