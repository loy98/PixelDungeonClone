#include "GameScene.h"
#include "CommonFunction.h"
#include "Level.h"
#include "UIManager.h"
#include "GameOver.h"

HRESULT GameScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);
	currLevel = testLevel;
	LevelInit(currLevel);

	UIManager::GetInstance()->GetUiGameOver()->SetRestartCallBack([this]() {
		this->Restart();         // 함수 등록 (람다)
		});

	UIManager::GetInstance()->GetUiGameOver()->SetExitCallBack([]() {
		PostQuitMessage(0); });

	// 시스템 로그-던전 입장
	wstring floor = to_wstring(testLevel);
	// 한글
	wstring kor = L"던전의 " + floor  + L"층으로 내려왔다.";
	UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::Yellow));
	// 영어
	//wstring eng = L"Welcome to the level " + floor + L" of Pixel Dungeon!";
	//UIManager::GetInstance()->SendLog(eng, D2D1::ColorF(D2D1::ColorF::White));

	return S_OK;
}

void GameScene::Release()
{
	for (auto& l : levels) {
		if (l != nullptr) {
			l->Release();
			delete l;
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

void GameScene::Restart()
{
	for (auto& l : levels) {
		if (l != nullptr)
		{
			l->Release();
			delete l;
			l = nullptr;
		}
	}

	currLevel = testLevel;
	LevelInit(currLevel);

	UIManager::GetInstance()->GetUiGameOver()->SetRestartCallBack([this]() {
		this->Restart(); UIManager::GetInstance()->SendLog(L"Click", D2D1::ColorF(D2D1::ColorF::Blue)); });

	UIManager::GetInstance()->GetUiGameOver()->SetExitCallBack([]() {
		PostQuitMessage(0); });

}

GameScene::GameScene()
{
	for (auto& l : levels) {
		l = nullptr;
	}
}

void GameScene::LevelInit(int testLevel)
{
	levels[testLevel] = new Level;
	if (levels[testLevel]) {
		levels[testLevel]->Init();
		UIManager::GetInstance()->SendLog(
		L"Wellcom To " + to_wstring(testLevel + 1) + L" of Pixel Dungeon!",
		D2D1::ColorF(D2D1::ColorF::White));
		wstring kor = L"던전의 " + to_wstring(testLevel + 1) + L"층으로 내려왔다.";
		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::Yellow));
	}
}
