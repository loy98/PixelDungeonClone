#pragma once
#include "Item.h"
enum EquipType
{
	WEAPON,
	ARMOR,
	SHIELD
};
class Equipment : public Item
{
protected:
	int bonusValue;
	EquipType eqType;
public:
	Equipment();
	virtual ~Equipment();

	virtual void Equip(Entity* player);
	virtual void UnEquip(Entity* player);
	EquipType GetEqType() { return eqType; }
	int GetBonusValue() { return bonusValue; }
};

