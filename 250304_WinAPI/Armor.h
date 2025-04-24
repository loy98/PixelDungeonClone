#pragma once
#include "Equipment.h"
class Armor : public Equipment
{
private:


public:
	Armor(FPOINT position, string name);
	virtual ~Armor();

	void Render(HDC hdc) override;
};

