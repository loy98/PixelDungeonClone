#include "Level.h"
#include "CommonFunction.h"

void Level::Init()
{

	nowZoomScale = 0.0f;

	player = nullptr;

	tempTileSize = 30;

	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {
			tempTile[20 * i + j] = 
			{	280+j*tempTileSize, 
				60+i*tempTileSize,
				280 + (j+1) * tempTileSize, 
				60 + (i+1) * tempTileSize 
			};
		}
	}

	BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	GreyBrush = CreateSolidBrush(RGB(100, 100, 100));
	WhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	RedBrush = CreateSolidBrush(RGB(255, 0, 0));



	for (auto& s : shouldBeRender) 
	{
		s = true;
	}

	for (auto& h : hasExplored) 
	{
		h = true;
	}

	for (auto& i : isSeen) 
	{
		i = true;
	}

	/*for (int i = 0; i < 5; ++i) {
		zoomedSampleTile[i] = ImageManager::GetInstance()->AddZoomedImage(
			"PixelDungeon_Tile0", L"Image/tiles0.bmp", 256, 64,
			SAMPLE_TILE_X, SAMPLE_TILE_Y, zoomScales[i]);
	}*/

	//Sleep(3000);
	FileLoad();

	/*for (int i = 0; i < TILE_Y; ++i) {
		for (int j = 0; j < TILE_X; ++j) {
			tiles[i * TILE_X + j] = new Tile(j, i, tileInfo[i * TILE_X + j].tileCode);
		}
	}*/

}

void Level::Release()
{
	DeleteObject(BlackBrush);
	DeleteObject(GreyBrush);
	DeleteObject(WhiteBrush);
	DeleteObject(RedBrush);
}

void Level::Update()
{
}

void Level::Render(HDC hdc)
{
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {
			
			switch (map[20 * i + j].type) {
				case TT::WALL :
					hOldBrush = (HBRUSH)SelectObject(hdc, GreyBrush);
					RenderRect(hdc, tempTile[20 * i + j]);
					SelectObject(hdc, hOldBrush);
					break;
				case TT::FLOOR:
					hOldBrush = (HBRUSH)SelectObject(hdc, WhiteBrush);
					RenderRect(hdc, tempTile[20 * i + j]);
					SelectObject(hdc, hOldBrush);
					break;
				case TT::NONE:
					hOldBrush = (HBRUSH)SelectObject(hdc, BlackBrush);
					RenderRect(hdc, tempTile[20 * i + j]);
					SelectObject(hdc, hOldBrush);
					break;
				default: 
					hOldBrush = (HBRUSH)SelectObject(hdc, RedBrush);
					RenderRect(hdc, tempTile[20 * i + j]);
					SelectObject(hdc, hOldBrush);
					break;
			}
		}
	}
}

void Level::FileLoad()
{
	// 파일 로드
	HANDLE hFile = CreateFile(
		L"TilemapDatas/TestMap_Heart.dat", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, TEXT("파일 열기 실패"), TEXT("경고"), MB_OK);
		return;
	}
	DWORD dwByte = 0;
	if (!ReadFile(hFile, map, sizeof(map), &dwByte, NULL))
	{
		MessageBox(g_hWnd, TEXT("파일 읽기 실패"), TEXT("경고"), MB_OK);
	}
	CloseHandle(hFile);
}

Level::Level()
{
}

Level::~Level()
{
}
