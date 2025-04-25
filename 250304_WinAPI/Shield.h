#pragma once
#include "Equipment.h"
class Shield : public Equipment
{
private:


public:
	Shield(FPOINT position, string name);
	virtual ~Shield();

	void Render(HDC hdc) override;
};

