#pragma once
#include "GameObject.h"
class Level;
class GameScene : public GameObject
{
private:
	Level* levels[1];
	int currLevel;
	const int testLevel = 0;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void Restart();

	GameScene();
	virtual ~GameScene() {};

protected:
	void LevelInit(int level);
};

