#pragma once
#include "config.h"

class D2DImage;
class LobbyBackground
{
private:
	D2DImage* currentGround;
	D2DImage* nextGround;
	D2DImage* currentUnderground;
	D2DImage* nextUnderground;

	float groundMoveSpeed;
	float undergroundMoveSpeed;

	float groundCameraY;
	float undergroundCameraY;

	float time;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);
};

