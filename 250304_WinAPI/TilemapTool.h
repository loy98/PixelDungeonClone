#pragma once
#include "GameObject.h"
#include "config.h"


class Image;
class Button;
class TilemapTool : public GameObject
{
private:
	Image* sampleTile;
	RECT rcSampleTile;

	TILE_INFO tileInfo[TILE_X * TILE_Y];
	RECT rcMain;

	POINT selectedTile;

	Button* saveButton;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Save();
	void Load();

	TilemapTool() {};
	virtual ~TilemapTool() {};
};

