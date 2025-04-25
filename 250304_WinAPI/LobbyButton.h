#pragma once
#include "Button.h"

class MapButton: public Button
{
public:
	using Super = Button;
	virtual HRESULT Init(int posX, int posY);
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

class ExitButton : public Button
{
public:
	using Super = Button;
	virtual HRESULT Init(int posX, int posY);
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

