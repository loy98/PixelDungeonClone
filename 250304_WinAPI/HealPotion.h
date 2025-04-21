#pragma once
#include "Consumable.h"


class HealPotion : public Consumable
{
private:
	int healAmount;

public:
	HealPotion();
	virtual ~HealPotion();

	void Use(Entity* user) override;

};

