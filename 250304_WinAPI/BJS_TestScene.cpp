#include "BJS_TestScene.h"
#include "Image.h"
#include "CommonFunction.h"
#include "config.h"

HRESULT BJS_TestScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, TILEMAPTOOL_Y);

	sampleTile = ImageManager::GetInstance()->AddImage(
		"배틀시티_샘플타일", L"Image/mapTiles.bmp", 640, 288,
		SAMPLE_TILE_X, SAMPLE_TILE_Y);

	backGround = new Image();
	if (FAILED(backGround->Init(TEXT("Image/BackGround.bmp"), WINSIZE_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/backGround.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}

	//Sleep(3000);
	Load();

	zoomScale = 0;

	testRc_DnD = { 300, 300, 600, 600 };

	isDragging = false;

	return S_OK;

}

void BJS_TestScene::Release()
{
	if (backGround)
	{
		backGround->Release();
		delete backGround;
		backGround = nullptr;
	}
}

void BJS_TestScene::Update()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("전투씬_1", "로딩_1");
	}

	// Wheel Input Test
	if (MouseManager::GetInstance()->GetWheelDelta() != 0) {
		short wd = MouseManager::GetInstance()->GetWheelDelta();
		if (wd > 0) {
			zoomScale += 0.5f;
		}
		else if (wd < 0) {
			zoomScale -= 0.5f;
		}
	}

	// DragNDrop Test
	if (PtInRect(&testRc_DnD, MouseManager::GetInstance()->GetMousePos())) {
		if (MouseManager::GetInstance()->GetIsDragging(MOUSE_LEFT))
		{
			long tempDeltaX = MouseManager::GetInstance()->GetDeltaX();
			long tempDeltaY = MouseManager::GetInstance()->GetDeltaY();

			testRc_DnD.left += tempDeltaX;
			testRc_DnD.right += tempDeltaX;
			testRc_DnD.top += tempDeltaY;
			testRc_DnD.bottom += tempDeltaY;

		}
	}
	
}

void BJS_TestScene::Render(HDC hdc)
{
	backGround->Render(hdc);
	// 메인 타일 영역
	for (int i = 0; i < TILE_X * TILE_Y; ++i)
	{
		sampleTile->FrameRender(hdc, tileInfo[i].rc.left,
			tileInfo[i].rc.top, tileInfo[i].frameX,
			tileInfo[i].frameY, false, false);
	}

	wsprintf(szText, TEXT("zoomScale : %d"), (int)zoomScale);
	TextOut(hdc, 100, 20, szText, wcslen(szText));

	wsprintf(szText, TEXT("deltaX : %d"), MouseManager::GetInstance()->GetDeltaX());
	TextOut(hdc, 100, 100, szText, wcslen(szText));

	RenderRect(hdc, testRc_DnD);
}

void BJS_TestScene::Load()
{
	// 파일 로드
	HANDLE hFile = CreateFile(
		L"TileMapData.dat", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, TEXT("파일 열기 실패"), TEXT("경고"), MB_OK);
		return;
	}
	DWORD dwByte = 0;
	if (!ReadFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL))
	{
		MessageBox(g_hWnd, TEXT("파일 읽기 실패"), TEXT("경고"), MB_OK);
	}
	CloseHandle(hFile);

}
