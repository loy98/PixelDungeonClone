#pragma once
#include "GameObject.h"

class D2DImage;
class BattleScene : public GameObject
{
private:

	//level 
	D2DImage* sampleTile;
	TILE_INFO tileInfo[TILE_X * TILE_Y];

	
	D2DImage* test;
	
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Load();

	BattleScene() = default;
	virtual ~BattleScene() override = default;
};

