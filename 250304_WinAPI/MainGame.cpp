#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Timer.h"
#include "BattleScene.h"
#include "TilemapTool.h"
#include "AstarScene.h"
#include "LoadingScene.h"
#include "D2DImage.h"
#include "D2DImageManager.h"

HRESULT MainGame::Init()
{
#pragma region Manager_Initialze
	ImageManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
#pragma endregion

#pragma region Scene_Initialize
	SceneManager::GetInstance()->AddScene("A*æÀ∞Ì∏Æ¡Ú", new AstarScene());
	SceneManager::GetInstance()->AddScene("¿¸≈ıæ¿_1", new BattleScene());
	SceneManager::GetInstance()->AddScene("≈∏¿œ∏ ≈¯", new TilemapTool());
	SceneManager::GetInstance()->AddLoadingScene("∑Œµ˘_1", new LoadingScene());
	SceneManager::GetInstance()->ChangeScene("≈∏¿œ∏ ≈¯");
#pragma endregion
	
	hdc = GetDC(g_hWnd);


	return S_OK;
}

void MainGame::Release()
{
	SceneManager::GetInstance()->Release();
	KeyManager::GetInstance()->Release();
	ImageManager::GetInstance()->Release();

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	SceneManager::GetInstance()->Update();
	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	D2DImage::BeginDraw();
	D2DImage::Clear(D2D1::ColorF(D2D1::ColorF::Black));


	SceneManager::GetInstance()->Render(hdc);

	
	D2DImage::EndDraw();
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'a': case 'A':
			SceneManager::GetInstance()->ChangeScene("¿¸≈ıæ¿_1");
			break;
		case 'd': case 'D':
			SceneManager::GetInstance()->ChangeScene("≈∏¿œ∏ ≈¯");
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);

		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}
