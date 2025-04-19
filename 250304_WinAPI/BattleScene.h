#pragma once
#include "GameObject.h"

class Game;
class Image;
class BattleScene : public GameObject
{
private:
	Image* backGround;
	Image* sampleTile;
	TILE_INFO tileInfo[TILE_X * TILE_Y];
	Game* testGame;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Load();

	BattleScene() {};
	virtual ~BattleScene() {};
};

