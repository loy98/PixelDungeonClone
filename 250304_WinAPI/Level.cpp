#include "Level.h"
#include "DungeonGenerator.h"
#include "config.h"
#include <random>

#include "CommonFunction.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "Entity.h"
#include "IntegratedDungeonSystem.h"
#include "Monster.h"
#include "Player.h"
#include "TurnManager.h"

void Level::Init()
{
    turnManager = new TurnManager();

    sampleTile = D2DImageManager::GetInstance()->AddImage(
        "배틀시티_샘플타일", L"Image/tiles_sewers.png",
        16, 16);

    tempTileSize = 30;

    for (int i = 0; i < TILE_Y; ++i)
    {
        for (int j = 0; j < TILE_X; ++j)
        {
            tempTile[TILE_X * i + j] =
            {
                GRID_POS_OFFSET.x + j * tempTileSize,
                GRID_POS_OFFSET.y + i * tempTileSize,
                GRID_POS_OFFSET.x + (j + 1) * tempTileSize,
                GRID_POS_OFFSET.y + (i + 1) * tempTileSize
            };
        }
    }

    mapRc = {tempTile[0].left, tempTile[0].top, tempTile[399].right, tempTile[399].bottom};

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

    // Initialize frame map data
    std::vector<std::vector<POINT>> frameMapData;
    frameMapData.resize(50);  // Reserve space for all tile types

    // Water animation frames
    frameMapData[36] = {
        {10, 1}, {11, 1}, {12, 1}, {13, 1}  // Water animation sequence
    };

    // Torch animation frames
    frameMapData[31] = {
        {5, 1}, {5, 2}, {6, 2}, {5, 2}  // Torch animation sequence
    };

    // Door animation frames (for opening/closing)
    frameMapData[2] = {
        {8, 3}, {9, 3}, {10, 3}, {11, 3}  // Door animation sequence
    };

    // Set the frame map data
    SetFrameMapData(frameMapData);

    FileLoad();

    // Initialize turn manager
    turnManager = new TurnManager();

    // Set map dimensions
    mapWidth = TILE_X;
    mapHeight = TILE_Y;

    // Generate dungeon
    dungeonSystem.GenerateDungeon(this, mapWidth, mapHeight, 1);
    
    // Place player
    FPOINT playerPos = GetRandomFloorTile();
    Player* player = new Player(playerPos, 50.0f);
    AddActor(player);

    // Place monsters
    for (int i = 0; i < 5; i++)
    {
        FPOINT monsterPos = GetRandomFloorTile();
        Monster* monster = new Monster(monsterPos, 3.0f);
        AddActor(monster);
    }

    // Add actors to turn manager
    for (auto actor : actors)
    {
        if (actor)
            turnManager->AddActor(actor);
    }

    frameTimer = 0.0f;  // Initialize frame timer
}

void Level::Release()
{
    for (auto actor : actors)
    {
        if (actor)
        {
            delete actor;
            actor = nullptr;
        }
    }
}

void Level::Update()
{
    if (PtInRect(&mapRc, MouseManager::GetInstance()->GetDragEndP()))
    {
        if (MouseManager::GetInstance()->GetValueUsed() == false)
        {
            long posX = MouseManager::GetInstance()->GetDragEndP().x;
            long posY = MouseManager::GetInstance()->GetDragEndP().y;

            long indX = (posX - mapRc.left) / tempTileSize;
            long indY = (posY - mapRc.top) / tempTileSize;

            if (indX >= 0 && indX < TILE_X && indY >= 0 && indY < TILE_Y)
            {
                map[indY * TILE_X + indX].type = static_cast<int>(TT::COUNT);
            }

            MouseManager::GetInstance()->InitPoints();
            MouseManager::GetInstance()->AlreadyUsed();
        }
    }

    if (MouseManager::GetInstance()->GetIsDragging(MOUSE_LEFT))
    {
        long tempDeltaX = MouseManager::GetInstance()->GetDeltaX();
        long tempDeltaY = MouseManager::GetInstance()->GetDeltaY();

        for (auto& t : tempTile)
        {
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

    // Update frame animation
    UpdateFrameAnimation(TimerManager::GetInstance()->GetDeltaTime());

    turnManager->ProcessTurns(this);
    dungeonSystem.UpdateEnvironmentalEffects(this, TimerManager::GetInstance()->GetDeltaTime());
}

void Level::Render(HDC hdc)
{
    sampleTile->DrawRect({(float)mapRc.left, (float)mapRc.top}, {(float)mapRc.right, (float)mapRc.bottom}, 1, 1);

    for (int i = 0; i < TILE_Y; ++i)
    {
        for (int j = 0; j < TILE_X; ++j)
        {
            int tileType = map[TILE_X * i + j].type;
            int tileX = static_cast<int>(tempTile[TILE_X * i + j].left);
            int tileY = static_cast<int>(tempTile[TILE_X * i + j].top);

            // Get the current frame coordinates for this tile type
            POINT frame = GetCurrentFrame(tileType);
            
            // Render the tile using the frame coordinates
            sampleTile->Middle_RenderFrameScale(tileX, tileY, 2.f, 2.f, frame.x, frame.y);
        }
    }

    // Render actors
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

void Level::GenerateMap(int width, int height)
{
    // 맵 생성
    mapData = dungeonGenerator.Generate(width, height);

    // Map 객체 배열 초기화 (만약 사용한다면)
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;
            // Map 객체 설정
            map[index].type = mapData[y][x];
        }
    }

    // 생성된 맵을 기반으로 tempTile 및 shouldBeRender, hasExplored, isSeen 배열 초기화
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;
            // 맵 타일 위치 설정 (기존 코드 참고)
            tempTile[index] = {
                GRID_POS_OFFSET.x + x * tempTileSize,
                GRID_POS_OFFSET.y + y * tempTileSize,
                GRID_POS_OFFSET.x + (x + 1) * tempTileSize,
                GRID_POS_OFFSET.y + (y + 1) * tempTileSize
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
        tempTile[(height - 1) * width + (width - 1)].right,
        tempTile[(height - 1) * width + (width - 1)].bottom
    };
}

int Level::GetTileType(int x, int y) const
{
    // 좌표가 맵 범위 내인지 확인
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
    {
        return mapData[y][x];
    }
    return 0; // 맵 범위 밖은 벽으로 처리
}

bool Level::IsSolid(int x, int y) const
{
    int tileType = GetTileType(x, y);
    return tileType == 0; // TILE_WALL인 경우 통과 불가
}

FPOINT Level::GetRandomFloorTile() const
{
    std::vector<FPOINT> floorTiles;

    // Check if map is initialized
    if (mapData.empty() || mapHeight <= 0 || mapWidth <= 0) {
        // Return a default position if map is not initialized
        return {static_cast<float>(GRID_POS_OFFSET.x + tempTileSize), 
                static_cast<float>(GRID_POS_OFFSET.y + tempTileSize)};
    }

    // 모든 바닥 타일 위치 수집
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            int tileType = mapData[y][x];
            // Check for any floor tile type (including variations)
            if (tileType == 1 || // TILE_FLOOR
                (tileType >= 20 && tileType <= 23)) // TILE_FLOOR_NORMAL to TILE_FLOOR_MOSSY
            {
                FPOINT pos;
                pos.x = static_cast<float>(GRID_POS_OFFSET.x + x * tempTileSize + tempTileSize / 2);
                pos.y = static_cast<float>(GRID_POS_OFFSET.y + y * tempTileSize + tempTileSize / 2);
                floorTiles.push_back(pos);
            }
        }
    }

    // If no floor tiles found, return a default position
    if (floorTiles.empty()) {
        return {static_cast<float>(GRID_POS_OFFSET.x + tempTileSize), 
                static_cast<float>(GRID_POS_OFFSET.y + tempTileSize)};
    }

    // 랜덤하게 하나 선택
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, floorTiles.size() - 1);
    return floorTiles[dis(gen)];
}

// Add GetCurrentFrame implementation
POINT Level::GetCurrentFrame(int tileType) const
{
    // // If we have frame data for this tile type
    // if (tileType >= 0 && tileType < frameMap.size() && !frameMap[tileType].empty())
    // {
    //     // Calculate current frame index based on time
    //     int frameIndex = static_cast<int>((frameTimer / FRAME_CHANGE_TIME) * frameMap[tileType].size()) % frameMap[tileType].size();
    //     return frameMap[tileType][frameIndex];
    // }

    // Default frames for basic tiles if no frame data exists
    switch (tileType)
    {
    case 0:  // Wall
        return {0, 3};
    case 1:  // Floor
        return {0, 0};
    case 2:  // Door
        return {8, 3};
    case 3:  // Entrance
        return {0, 1};
    case 4:  // Exit
        return {1, 1};
    case 5:  // Hidden door
        return {13, 3};
    case 6:  // Empty space
        return {0, 9};
    case 10: // Top wall
        return {0, 6};
    case 11: // Bottom wall
        return {0, 12};
    case 12: // Left wall
        return {2, 9};
    case 13: // Right wall
        return {4, 9};
    case 14: // Top-left corner
        return {9, 6};
    case 15: // Top-right corner
        return {6, 3};
    case 16: // Bottom-left corner
        return {3, 13};
    case 17: // Bottom-right corner
        return {0, 13};
    default:
        return {0, 5}; // Default/unknown tile
    }
}
