#pragma once
#include "GameObject.h"



class MainGame : public GameObject
{
private:
	HDC hdc;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	void Render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();
};

