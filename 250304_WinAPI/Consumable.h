#pragma once
#include "Item.h"
class Consumable : public Item
{
public:
	Consumable() { this->itemType = ItemType::CONSUMABLE; }
	virtual ~Consumable() {};
};

