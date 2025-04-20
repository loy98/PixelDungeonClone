#include "Level.h"

#include <random>

#include "CommonFunction.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "Entity.h"
#include "Monster.h"
#include "Player.h"
#include "TurnManager.h"

void Level::Init()
{
	turnManager = new TurnManager();

	sampleTile = D2DImageManager::GetInstance()->AddImage(
		"배틀시티_샘플타일", L"Image/tiles_sewers.png",
		16, 16);

	
	// nowZoomScale = 0.0f;

	tempTileSize = 30;

	for (int i = 0; i < TILE_Y; ++i) {
		for (int j = 0; j < TILE_X; ++j) {
			tempTile[TILE_X * i + j] = 
			{	GRID_POS_OFFSET.x + j*tempTileSize, 
				GRID_POS_OFFSET.y + i*tempTileSize,
				GRID_POS_OFFSET.x + (j+1) * tempTileSize, 
				GRID_POS_OFFSET.y + (i+1) * tempTileSize 
			};
		}
	}

	mapRc = { tempTile[0].left, tempTile[0].top, tempTile[399].right, tempTile[399].bottom };

	// BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	// GreyBrush = CreateSolidBrush(RGB(100, 100, 100));
	// WhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	// RedBrush = CreateSolidBrush(RGB(255, 0, 0));



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

	FileLoad();

	// 턴 매니저 초기화
	turnManager = new TurnManager();
    
	// 맵 크기 설정
	mapWidth = TILE_X;
	mapHeight = TILE_Y;
    
	// 절차적 맵 생성
	GenerateMap(mapWidth, mapHeight);
    
	// 플레이어 배치
	FPOINT playerPos = GetRandomFloorTile();
	Player* player = new Player(playerPos, 5.0f); // 속도 5.0f 가정
	AddActor(player);
	// 플레이어 추가 코드...
    
	// 몬스터 배치
	for (int i = 0; i < 5; i++) { // 5마리 몬스터 예시
		FPOINT monsterPos = GetRandomFloorTile();
		Monster* monster = new Monster(monsterPos, 3.0f); // 속도 3.0f 가정
		AddActor(monster);
	}
	
	// // 시작 위치 테스트용 매직넘버
	// Entity* player = new Player(GetPosByGridIndex(3, 3), 100.f);
	// Entity* monster1 = new Monster(GetPosByGridIndex(5, 4), 100.f);
	// Entity* monster2 = new Monster(GetPosByGridIndex(4, 5), 100.f);

	// AddActor(player);
	// AddActor(monster1);
	// AddActor(monster2);

	for (auto actor : actors)
	{
		if (actor)
			turnManager->AddActor(actor);
	}
	
}

void Level::Release()
{
	// DeleteObject(BlackBrush);
	// DeleteObject(GreyBrush);
	// DeleteObject(WhiteBrush);
	// DeleteObject(RedBrush);
	
	for (auto actor : actors)
	{
		if (actor)
		{
			// actor->Release();
			delete actor;
			actor = nullptr;
		}
	}
}

void Level::Update()
{
	/*if (MouseManager::GetInstance()->GetValueUsed() == false) {
		POINT p = MouseManager::GetInstance()->GetMousePos();

		if (PtInRect(&mapRc, p)) {
			long indX = (p.x - mapRc.left) / tempTileSize;
			long indY = (p.y - mapRc.top) / tempTileSize;

			map[indY * 20 + indX].type = TT::COUNT;
		}

		MouseManager::GetInstance()->AlreadyUsed();
	}*/
	if (PtInRect(&mapRc, MouseManager::GetInstance()->GetDragEndP())) 
	{
		if (MouseManager::GetInstance()->GetValueUsed() == false) {

			long posX = MouseManager::GetInstance()->GetDragEndP().x;
			long posY = MouseManager::GetInstance()->GetDragEndP().y;

			long indX = (posX - mapRc.left) / tempTileSize;
			long indY = (posY - mapRc.top) / tempTileSize;

			if (indX >= 0 && indX < TILE_X && indY >= 0 && indY < TILE_Y)	///
			{														/// 구현 하고 싶은 로직 넣는 부분
				map[indY * TILE_X + indX].type = static_cast<int>(TT::COUNT);				///
			}														///

			MouseManager::GetInstance()->InitPoints();
			MouseManager::GetInstance()->AlreadyUsed();
		}
	} ///디버깅을 위해 마우스 왼쪽 버튼을 떼면 그 자리에 있는 타일이 빨간색으로 변하게 해놨습니다. 
	  ///맵으로 사용하실 땐 타일 선택 로직(이동 및 공격)을 써주세요!
	
	if (MouseManager::GetInstance()->GetIsDragging(MOUSE_LEFT))
	{
		long tempDeltaX = MouseManager::GetInstance()->GetDeltaX();
		long tempDeltaY = MouseManager::GetInstance()->GetDeltaY();

		for (auto& t : tempTile) {
			t.left += tempDeltaX;
			t.right += tempDeltaX;
			t.top += tempDeltaY;
			t.bottom += tempDeltaY;
		}

		mapRc.left += tempDeltaX;
		mapRc.right += tempDeltaX;
		mapRc.top += tempDeltaY;
		mapRc.bottom += tempDeltaY;

	}

	turnManager->ProcessTurns(this);

}

void Level::Render(HDC hdc)
{
	// PatBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);

	// RenderRect(hdc, mapRc);
	sampleTile->DrawRect({(float)mapRc.left, (float)mapRc.top}, {(float)mapRc.right, (float)mapRc.bottom}, 1, 1);
	
	for (int i = 0; i < TILE_Y; ++i) {
		for (int j = 0; j < TILE_X; ++j) {
			
			switch (map[TILE_X * i + j].type) {
				case 0:
					sampleTile->RenderFrameScale(static_cast<int>(tempTile[TILE_X * i + j].left),
						static_cast<int>(tempTile[TILE_X * i + j].top), 2.f, 2.f, 0, 3);
					// hOldBrush = (HBRUSH)SelectObject(hdc, GreyBrush);
					// RenderRect(hdc, tempTile[20 * i + j]);
					// SelectObject(hdc, hOldBrush);
					break;
				case 1:
					sampleTile->RenderFrameScale(static_cast<int>(tempTile[TILE_X * i + j].left),
						static_cast<int>(tempTile[TILE_X * i + j].top),2.f, 2.f, 0, 0);
					// hOldBrush = (HBRUSH)SelectObject(hdc, WhiteBrush);
					// RenderRect(hdc, tempTile[20 * i + j]);
					// SelectObject(hdc, hOldBrush);
					break;
			case 2:
			case 3:
			case 4:
			case 5:
				// case static_cast<int>(TT::NONE):
				// 	sampleTile->RenderFrame(static_cast<int>(tempTile[20 * i + j].left),
				// 		static_cast<int>(tempTile[20 * i + j].top), 0, 0);
				// 	// hOldBrush = (HBRUSH)SelectObject(hdc, BlackBrush);
				// 	// RenderRect(hdc, tempTile[20 * i + j]);
				// 	// SelectObject(hdc, hOldBrush);
				// 	break;
				default:
					sampleTile->RenderFrameScale(static_cast<int>(tempTile[TILE_X * i + j].left),
						static_cast<int>(tempTile[TILE_X * i + j].top),2.f, 2.f, 0, 9); 
					// hOldBrush = (HBRUSH)SelectObject(hdc, RedBrush);
					// RenderRect(hdc, tempTile[20 * i + j]);
					// SelectObject(hdc, hOldBrush);
					break;
			}
		}
	}

	for (auto actor : actors)
	{
		actor->Render(hdc);
	}
}

void Level::FileLoad()
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


void Level::AddActor(Entity* actor)
{
	// 추가하려는 Entity가 이미 container에 있다면 return
	auto it = find(actors.begin(), actors.end(), actor);
	if (it != actors.end())
		return;

	actors.push_back(actor);
}

void Level::GenerateMap(int width, int height) {
	// 맵 생성
	mapData = dungeonGenerator.Generate(width, height);
    
	// Map 객체 배열 초기화 (만약 사용한다면)
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = y * width + x;
			// Map 객체 설정
			map[index].type = mapData[y][x];
		}
	}
	
    // // 맵 생성
    // mapData = dungeonGenerator.Generate(width, height);
    //
    // 생성된 맵을 기반으로 tempTile 및 shouldBeRender, hasExplored, isSeen 배열 초기화
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            // 맵 타일 위치 설정 (기존 코드 참고)
            tempTile[index] = {
                GRID_POS_OFFSET.x + x * tempTileSize,
                GRID_POS_OFFSET.y + y * tempTileSize,
                GRID_POS_OFFSET.x + (x+1) * tempTileSize,
                GRID_POS_OFFSET.y + (y+1) * tempTileSize
            };
            
            // 타일 속성 초기화
            shouldBeRender[index] = true;
            hasExplored[index] = false;
            isSeen[index] = false;
        }
    }
    
    // 맵 경계 설정
    mapRc = {
        tempTile[0].left,
        tempTile[0].top,
        tempTile[(height-1) * width + (width-1)].right,
        tempTile[(height-1) * width + (width-1)].bottom
    };
}

int Level::GetTileType(int x, int y) const {
    // 좌표가 맵 범위 내인지 확인
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
        return mapData[y][x];
    }
    return 0; // 맵 범위 밖은 벽으로 처리
}

bool Level::IsSolid(int x, int y) const {
    int tileType = GetTileType(x, y);
    return tileType == 0; // TILE_WALL인 경우 통과 불가
}

FPOINT Level::GetRandomFloorTile() const {
    std::vector<FPOINT> floorTiles;
    
    // 모든 바닥 타일 위치 수집
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            if (mapData[y][x] == 1) { // TILE_FLOOR
                FPOINT pos;
                pos.x = GRID_POS_OFFSET.x + x * tempTileSize + tempTileSize / 2;
                pos.y = GRID_POS_OFFSET.y + y * tempTileSize + tempTileSize / 2;
                floorTiles.push_back(pos);
            }
        }
    }
    
    // 랜덤 바닥 타일 선택
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, floorTiles.size() - 1);
    
    return floorTiles[dist(gen)];
}