#pragma once
#include "Item.h"
enum class EquipType
{
	WEAPON,
	ARMOR,
	SHIELD
};
class Equipment : public Item
{
private:
	int bonusValue;
	EquipType eqType;
public:
	Equipment();
	virtual ~Equipment();

	virtual void Equip(Entity* player);
	virtual void UnEquip(Entity* player);
};

