#include "GameScene.h"
#include "CommonFunction.h"
#include "Level.h"

HRESULT GameScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);
	currLevel = testLevel;
	levels[testLevel] = new Level;
	if (levels[testLevel]) {
		levels[testLevel]->Init();
	}
	

	return S_OK;
}

void GameScene::Release()
{
	for (auto& l : levels) {
		if (l != nullptr) {
			l->Release();
			l = nullptr;
		}
	}
}

void GameScene::Update()
{
	levels[currLevel]->Update();
}

void GameScene::Render(HDC hdc)
{
	levels[currLevel]->Render(hdc);
}

GameScene::GameScene()
{
	for (auto& l : levels) {
		l = nullptr;
	}
}
