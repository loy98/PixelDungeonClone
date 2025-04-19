#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Button.h"
#include <functional>

HRESULT TilemapTool::Init()
{
	SetClientRect(g_hWnd, TILEMAPTOOL_X, TILEMAPTOOL_Y);

	sampleTile = ImageManager::GetInstance()->AddImage(
		"배틀시티_샘플타일", L"Image/mapTiles.bmp", 640, 288,
		SAMPLE_TILE_X, SAMPLE_TILE_Y);

	// 샘플 타일 영역
	rcSampleTile.left = TILEMAPTOOL_X - sampleTile->GetWidth();
	rcSampleTile.top = 0;
	rcSampleTile.right = TILEMAPTOOL_X;
	rcSampleTile.bottom = sampleTile->GetHeight();

	tempTileSize = 30;

	//Load();

	// 메인 타일 영역
	rcMain.left = 0;
	rcMain.top = 0;
	rcMain.right = TILE_X * tempTileSize;
	rcMain.bottom = TILE_Y * tempTileSize;

	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {
			tempTile[20 * i + j] =
			{ rcMain.left + j * tempTileSize,
				rcMain.top + i * tempTileSize,
				rcMain.left + (j + 1) * tempTileSize,
				rcMain.top + (i + 1) * tempTileSize
			};
		}
	}

	tBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	tGreyBrush = CreateSolidBrush(RGB(100, 100, 100));
	tWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	tRedBrush = CreateSolidBrush(RGB(255, 0, 0));

	// UI - 버튼
	saveButton = new Button();
	saveButton->Init(
		TILEMAPTOOL_X - sampleTile->GetWidth() + 180,
		sampleTile->GetHeight() + 100);
	//saveButton->SetFunction(&TilemapTool::Save, this);
	//saveButton->SetFunction(std::bind(&TilemapTool::Save, this));
	saveButton->SetFunction([this]() {
		this->Save();
		});

	return S_OK;
}

void TilemapTool::Release()
{
	if (saveButton)
	{
		saveButton->Release();
		delete saveButton;
		saveButton = nullptr;
	}

	DeleteObject(tBlackBrush);
	DeleteObject(tGreyBrush);
	DeleteObject(tWhiteBrush);
	DeleteObject(tRedBrush);
}

void TilemapTool::Update()
{
	if (PtInRect(&rcSampleTile, g_ptMouse))
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x - rcSampleTile.left;
			int posY = g_ptMouse.y - rcSampleTile.top;
			selectedTile.x = posX / TILE_SIZE;
			selectedTile.y = posY / TILE_SIZE;
		}
	}
	else if (PtInRect(&rcMain, g_ptMouse))
	{
		if (KeyManager::GetInstance()->IsStayKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x;
			int posY = g_ptMouse.y;
			int tileX = posX / tempTileSize;
			int tileY = posY / tempTileSize;
			tileInfo[tileY * TILE_X + tileX].type = TT::FLOOR;
		}

		if (KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON))
		{
			int posX = g_ptMouse.x;
			int posY = g_ptMouse.y;
			int tileX = posX / tempTileSize;
			int tileY = posY / tempTileSize;
			tileInfo[tileY * TILE_X + tileX].type = TT::WALL;
		}

		if (KeyManager::GetInstance()->IsStayKeyDown(VK_MBUTTON))
		{
			int posX = g_ptMouse.x;
			int posY = g_ptMouse.y;
			int tileX = posX / tempTileSize;
			int tileY = posY / tempTileSize;
			tileInfo[tileY * TILE_X + tileX].type = TT::NONE;
		}
	}

	if (saveButton)	saveButton->Update();
}

void TilemapTool::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, TILEMAPTOOL_X, TILEMAPTOOL_Y, WHITENESS);

	// 메인 타일 영역
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {

			switch (tileInfo[20 * i + j].type) {
			case TT::WALL:
				hOldBrush = (HBRUSH)SelectObject(hdc, tGreyBrush);
				RenderRect(hdc, tempTile[20 * i + j]);
				SelectObject(hdc, hOldBrush);
				break;
			case TT::FLOOR:
				hOldBrush = (HBRUSH)SelectObject(hdc, tWhiteBrush);
				RenderRect(hdc, tempTile[20 * i + j]);
				SelectObject(hdc, hOldBrush);
				break;
			case TT::NONE:
				hOldBrush = (HBRUSH)SelectObject(hdc, tBlackBrush);
				RenderRect(hdc, tempTile[20 * i + j]);
				SelectObject(hdc, hOldBrush);
				break;
			default:
				hOldBrush = (HBRUSH)SelectObject(hdc, tRedBrush);
				RenderRect(hdc, tempTile[20 * i + j]);
				SelectObject(hdc, hOldBrush);
				break;
			}
		}
	}

	// 샘플 타일 영역
	sampleTile->Render(hdc, TILEMAPTOOL_X - sampleTile->GetWidth(), 0);

	// 선택된 타일
	sampleTile->FrameRender(hdc, 
		TILEMAPTOOL_X - sampleTile->GetWidth(),
		sampleTile->GetHeight() + 100,
		selectedTile.x, selectedTile.y, false, false);

	if (saveButton) saveButton->Render(hdc);
}

void TilemapTool::Save()
{
	// 파일 저장
	HANDLE hFile = CreateFile(
		L"TileMapData.dat", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, TEXT("파일 생성 실패"), TEXT("경고"), MB_OK);
		return;
	}
	DWORD dwByte = 0;
	WriteFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL);
	CloseHandle(hFile);
}

void TilemapTool::Load()
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
