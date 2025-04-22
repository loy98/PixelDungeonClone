// TileVariationManager.cpp
#include "TileVariationManager.h"
#include <vector>

TileVariationManager::TileVariationManager() {
}

TileVariationManager::~TileVariationManager() {
}

// 타일 맵 전체 처리 함수
std::vector<std::vector<POINT>> TileVariationManager::ProcessTileMap(const std::vector<std::vector<int>>& map) {
    int height = map.size();
    int width = map[0].size();
    
    // 결과 맵 초기화 (각 타일의 프레임 좌표)
    std::vector<std::vector<POINT>> frameMap(height, std::vector<POINT>(width, {0, 0}));
    
    // 모든 타일 처리
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tileType = map[y][x];
            int variation = 0;
            
            // 타일 유형에 따라 변형 결정
            switch (tileType) {
                case TILE_WALL:
                    variation = DetermineWallVariation(map, x, y);
                    break;
                case TILE_FLOOR:
                    variation = DetermineFloorVariation(map, x, y);
                    break;
                case TILE_DOOR:
                case TILE_HIDDEN_DOOR:
                    variation = DetermineDoorVariation(map, x, y);
                    break;
                // 다른 타일 유형은 기본 변형 사용
            }
            
            // 프레임 좌표 설정
            frameMap[y][x] = GetTileFrameCoordinates(tileType, variation);
        }
    }
    
    return frameMap;
}

// 벽 타일 변형 결정 함수
int TileVariationManager::DetermineWallVariation(const std::vector<std::vector<int>>& map, int x, int y) {
    // 주변 8방향의 벽 타일 상태를 비트마스크로 인코딩
    int bitmask = CreateSurroundingBitmask(map, x, y, TILE_WALL);
    
    // 주변 4방향(상하좌우)의 벽 여부 확인
    bool hasTop = IsTileOfType(map, x, y - 1, TILE_WALL);
    bool hasRight = IsTileOfType(map, x + 1, y, TILE_WALL);
    bool hasBottom = IsTileOfType(map, x, y + 1, TILE_WALL);
    bool hasLeft = IsTileOfType(map, x - 1, y, TILE_WALL);
    
    // 연결된 벽의 수
    int connectionCount = (hasTop ? 1 : 0) + (hasRight ? 1 : 0) + (hasBottom ? 1 : 0) + (hasLeft ? 1 : 0);
    
    // 벽 변형 결정
    if (connectionCount == 0) {
        return WALL_SINGLE; // 단일 벽
    } else if (connectionCount == 1) {
        if (hasTop) return WALL_TOP;
        if (hasRight) return WALL_RIGHT;
        if (hasBottom) return WALL_BOTTOM;
        if (hasLeft) return WALL_LEFT;
    } else if (connectionCount == 2) {
        if (hasTop && hasRight) return WALL_TOP_RIGHT;
        if (hasRight && hasBottom) return WALL_BOTTOM_RIGHT;
        if (hasBottom && hasLeft) return WALL_BOTTOM_LEFT;
        if (hasLeft && hasTop) return WALL_TOP_LEFT;
        if (hasTop && hasBottom) return WALL_VERTICAL;
        if (hasLeft && hasRight) return WALL_HORIZONTAL;
    } else if (connectionCount == 3) {
        if (!hasTop) return WALL_T_BOTTOM;
        if (!hasRight) return WALL_T_LEFT;
        if (!hasBottom) return WALL_T_TOP;
        if (!hasLeft) return WALL_T_RIGHT;
    } else {
        return WALL_CROSS; // 십자형 벽
    }
    
    return WALL_SINGLE; // 기본값
}

// 바닥 타일 변형 결정 함수
int TileVariationManager::DetermineFloorVariation(const std::vector<std::vector<int>>& map, int x, int y) {
    // 주변 8방향의 바닥이 아닌 타일 상태를 비트마스크로 인코딩
    int bitmask = CreateSurroundingBitmask(map, x, y, TILE_FLOOR);
    
    // 주변 4방향(상하좌우)의 바닥 여부 확인 (반전: 바닥이 아닌 경우 true)
    bool hasTop = !IsTileOfType(map, x, y - 1, TILE_FLOOR);
    bool hasRight = !IsTileOfType(map, x + 1, y, TILE_FLOOR);
    bool hasBottom = !IsTileOfType(map, x, y + 1, TILE_FLOOR);
    bool hasLeft = !IsTileOfType(map, x - 1, y, TILE_FLOOR);
    
    // 대각선 방향 확인
    bool hasTopRight = !IsTileOfType(map, x + 1, y - 1, TILE_FLOOR);
    bool hasBottomRight = !IsTileOfType(map, x + 1, y + 1, TILE_FLOOR);
    bool hasBottomLeft = !IsTileOfType(map, x - 1, y + 1, TILE_FLOOR);
    bool hasTopLeft = !IsTileOfType(map, x - 1, y - 1, TILE_FLOOR);
    
    // 바닥 변형 결정 (가장자리 우선)
    if (hasTop && !hasRight && !hasBottom && !hasLeft) {
        return FLOOR_EDGE_TOP;
    } else if (!hasTop && hasRight && !hasBottom && !hasLeft) {
        return FLOOR_EDGE_RIGHT;
    } else if (!hasTop && !hasRight && hasBottom && !hasLeft) {
        return FLOOR_EDGE_BOTTOM;
    } else if (!hasTop && !hasRight && !hasBottom && hasLeft) {
        return FLOOR_EDGE_LEFT;
    } 
    // 모서리 처리
    else if (hasTop && hasRight && !hasBottom && !hasLeft) {
        return FLOOR_CORNER_TOP_RIGHT;
    } else if (!hasTop && hasRight && hasBottom && !hasLeft) {
        return FLOOR_CORNER_BOTTOM_RIGHT;
    } else if (!hasTop && !hasRight && hasBottom && hasLeft) {
        return FLOOR_CORNER_BOTTOM_LEFT;
    } else if (hasTop && !hasRight && !hasBottom && hasLeft) {
        return FLOOR_CORNER_TOP_LEFT;
    }
    
    // 기본 바닥
    return FLOOR_NORMAL;
}

// 문 타일 변형 결정 함수
int TileVariationManager::DetermineDoorVariation(const std::vector<std::vector<int>>& map, int x, int y) {
    // 주변 4방향(상하좌우)의 바닥 여부 확인
    bool hasTopFloor = IsTileOfType(map, x, y - 1, TILE_FLOOR);
    bool hasRightFloor = IsTileOfType(map, x + 1, y, TILE_FLOOR);
    bool hasBottomFloor = IsTileOfType(map, x, y + 1, TILE_FLOOR);
    bool hasLeftFloor = IsTileOfType(map, x - 1, y, TILE_FLOOR);
    
    // 수직/수평 문 결정
    if ((hasTopFloor || hasBottomFloor) && !(hasLeftFloor || hasRightFloor)) {
        return DOOR_VERTICAL; // 수직 문
    } else {
        return DOOR_HORIZONTAL; // 수평 문 (기본값)
    }
}

// 타일 프레임 좌표 변환 함수
POINT TileVariationManager::GetTileFrameCoordinates(int tileType, int variation) {
    POINT frameCoord = {0, 0};
    
    switch (tileType) {
        case TILE_WALL:
            // 벽 타일 프레임 매핑
            // 예: 스프라이트 시트의 첫 번째 행에 벽 타일 변형이 있다고 가정
            frameCoord.y = 0;
            frameCoord.x = variation;
            break;
            
        case TILE_FLOOR:
            // 바닥 타일 프레임 매핑
            // 예: 스프라이트 시트의 두 번째 행에 바닥 타일 변형이 있다고 가정
            frameCoord.y = 1;
            frameCoord.x = variation;
            break;
            
        case TILE_DOOR:
            // 문 타일 프레임 매핑
            frameCoord.y = 2;
            frameCoord.x = variation;
            break;
            
        case TILE_HIDDEN_DOOR:
            // 숨겨진 문 타일 프레임 매핑
            frameCoord.y = 2;
            frameCoord.x = variation + 2; // 일반 문 다음에 위치한다고 가정
            break;
            
        case TILE_ENTRANCE:
            // 입구 타일 프레임
            frameCoord.y = 3;
            frameCoord.x = 0;
            break;
            
        case TILE_EXIT:
            // 출구 타일 프레임
            frameCoord.y = 3;
            frameCoord.x = 1;
            break;
            
        default:
            // 기본 타일 프레임
            frameCoord.y = 0;
            frameCoord.x = 0;
    }
    
    return frameCoord;
}

// 주변 타일 확인 함수
bool TileVariationManager::IsTileOfType(const std::vector<std::vector<int>>& map, int x, int y, int type) {
    // 맵 경계 확인
    if (x < 0 || y < 0 || y >= map.size() || x >= map[0].size()) {
        return false;
    }
    
    return map[y][x] == type;
}

// 비트마스크 생성 함수 (주변 8방향 타일 상태 인코딩)
int TileVariationManager::CreateSurroundingBitmask(const std::vector<std::vector<int>>& map, int x, int y, int targetType) {
    int bitmask = 0;
    
    // 8방향 확인
    for (int dir = 0; dir < 8; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        
        // 해당 방향의 타일이 목표 타입이면 비트 설정
        if (IsTileOfType(map, nx, ny, targetType)) {
            bitmask |= (1 << dir);
        }
    }
    
    return bitmask;
}
