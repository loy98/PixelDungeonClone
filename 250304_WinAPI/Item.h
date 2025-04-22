#pragma once
#include "config.h"

class Entity;
class D2DImage;

enum class ItemType
{
	CONSUMABLE,
	EQUIPMENT
};

class Item
{
private:
	FPOINT position;
	ItemType type;
	D2DImage* image;
public:
	Item();
	virtual ~Item();

	virtual void Use(Entity* user);
};

