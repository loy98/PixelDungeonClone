#include "Dungeon.h"
#include "CommonFunction.h"
#include "Level.h"

HRESULT Dungeon::Init()
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

void Dungeon::Release()
{
	for (auto& l : levels) {
		if (l != nullptr) {
			l->Release();
			l = nullptr;
		}
	}
}

void Dungeon::Update()
{
	levels[currLevel]->Update();
}

void Dungeon::Render(HDC hdc)
{
	levels[currLevel]->Render(hdc);
}
