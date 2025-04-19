#include "GameScene.h"
#include "CommonFunction.h"
#include "Level.h"

HRESULT GameScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);
	currLevel = testLevel;
	for (int i = 0; i < 6; ++i) {
		levels[i] = nullptr;
	}
	levels[testLevel] = new Level;
	levels[testLevel]->Init();

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
