#pragma once
#include "GameObject.h"

class D2DImage;
class BJS_TestScene : public GameObject
{
private:
	D2DImage* sampleTile;
	TILE_INFO tileInfo[TILE_X * TILE_Y];

	float zoomScale;
	wchar_t szText[128];

	RECT testRc_DnD;

	bool isDragging;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Load();

	BJS_TestScene() {};
	virtual ~BJS_TestScene() {};
};



