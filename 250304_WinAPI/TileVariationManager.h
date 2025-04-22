// TileVariationManager.h
#pragma once
#include "config.h"

class TileVariationManager {
public:
    TileVariationManager();
    ~TileVariationManager();

    // 타일 유형 열거형
    enum TileType {
        TILE_EMPTY = 0,     // 빈 공간
        TILE_WALL = 1,      // 벽
        TILE_FLOOR = 2,     // 바닥
        TILE_DOOR = 3,      // 문
        TILE_ENTRANCE = 4,  // 입구
        TILE_EXIT = 5,      // 출구
        TILE_HIDDEN_DOOR = 6 // 숨겨진 문
    };

    // 벽 타일 변형 유형 열거형
    enum WallVariation {
        WALL_SINGLE = 0,    // 단일 벽
        WALL_TOP = 1,       // 위쪽 벽
        WALL_RIGHT = 2,     // 오른쪽 벽
        WALL_BOTTOM = 3,    // 아래쪽 벽
        WALL_LEFT = 4,      // 왼쪽 벽
        WALL_TOP_RIGHT = 5, // 위-오른쪽 모서리
        WALL_BOTTOM_RIGHT = 6, // 아래-오른쪽 모서리
        WALL_BOTTOM_LEFT = 7, // 아래-왼쪽 모서리
        WALL_TOP_LEFT = 8,  // 위-왼쪽 모서리
        WALL_HORIZONTAL = 9, // 수평 벽
        WALL_VERTICAL = 10, // 수직 벽
        WALL_T_TOP = 11,    // T자형 위쪽
        WALL_T_RIGHT = 12,  // T자형 오른쪽
        WALL_T_BOTTOM = 13, // T자형 아래쪽
        WALL_T_LEFT = 14,   // T자형 왼쪽
        WALL_CROSS = 15     // 십자형 벽
    };

    // 바닥 타일 변형 유형 열거형
    enum FloorVariation {
        FLOOR_NORMAL = 0,   // 일반 바닥
        FLOOR_EDGE_TOP = 1, // 위쪽 가장자리
        FLOOR_EDGE_RIGHT = 2, // 오른쪽 가장자리
        FLOOR_EDGE_BOTTOM = 3, // 아래쪽 가장자리
        FLOOR_EDGE_LEFT = 4, // 왼쪽 가장자리
        FLOOR_CORNER_TOP_RIGHT = 5, // 위-오른쪽 모서리
        FLOOR_CORNER_BOTTOM_RIGHT = 6, // 아래-오른쪽 모서리
        FLOOR_CORNER_BOTTOM_LEFT = 7, // 아래-왼쪽 모서리
        FLOOR_CORNER_TOP_LEFT = 8  // 위-왼쪽 모서리
    };

    // 문 타일 변형 유형 열거형
    enum DoorVariation {
        DOOR_HORIZONTAL = 0, // 수평 문
        DOOR_VERTICAL = 1    // 수직 문
    };

    // 타일 변형 결정 함수
    int DetermineWallVariation(const std::vector<std::vector<int>>& map, int x, int y);
    int DetermineFloorVariation(const std::vector<std::vector<int>>& map, int x, int y);
    int DetermineDoorVariation(const std::vector<std::vector<int>>& map, int x, int y);

    // 타일 프레임 좌표 변환 함수
    POINT GetTileFrameCoordinates(int tileType, int variation);

    // 타일 맵 전체 처리 함수
    std::vector<std::vector<POINT>> ProcessTileMap(const std::vector<std::vector<int>>& map);

private:
    // 주변 타일 확인 함수
    bool IsTileOfType(const std::vector<std::vector<int>>& map, int x, int y, int type);
    
    // 비트마스크 생성 함수 (주변 8방향 타일 상태 인코딩)
    int CreateSurroundingBitmask(const std::vector<std::vector<int>>& map, int x, int y, int targetType);
    
    // 방향 상수
    const int DIR_TOP = 0;
    const int DIR_TOP_RIGHT = 1;
    const int DIR_RIGHT = 2;
    const int DIR_BOTTOM_RIGHT = 3;
    const int DIR_BOTTOM = 4;
    const int DIR_BOTTOM_LEFT = 5;
    const int DIR_LEFT = 6;
    const int DIR_TOP_LEFT = 7;
    
    // 방향별 오프셋
    const int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    const int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
};
