#include "WallTileRenderer.h"
#include <algorithm>

WallTileRenderer::WallTileRenderer() {
    // 부분 타일 위치 초기화
    InitializeSubTilePositions();
}

void WallTileRenderer::InitializeSubTilePositions() {
    // 타일맵에서 각 벽 타일 유형별 부분 타일 위치 초기화
    
    // 일반 벽 (기본 벽 타일) - 모든 부분이 동일한 벽 타일
    standardWallSubTiles[TOP_LEFT] = {0, 8};
    standardWallSubTiles[TOP_RIGHT] = {1, 8};
    standardWallSubTiles[BOTTOM_LEFT] = {0, 9};
    standardWallSubTiles[BOTTOM_RIGHT] = {1, 9};
    
    // 상단 벽 (바닥 위) - 상단은 일반 벽, 하단은 상단 벽 타일
    topWallSubTiles[TOP_LEFT] = {0, 12};
    topWallSubTiles[TOP_RIGHT] = {1, 12};
    topWallSubTiles[BOTTOM_LEFT] = {0, 13};
    topWallSubTiles[BOTTOM_RIGHT] = {1, 13};
    
    // 하단 벽 (바닥 아래) - 상단은 하단 벽 타일, 하단은 일반 벽
    bottomWallSubTiles[TOP_LEFT] = {0, 25};
    bottomWallSubTiles[TOP_RIGHT] = {1, 25};
    bottomWallSubTiles[BOTTOM_LEFT] = {0, 20};
    bottomWallSubTiles[BOTTOM_RIGHT] = {1, 20};
    
    // 좌측 벽 (바닥 왼쪽) - 좌측은 일반 벽, 우측은 좌측 벽 타일
    leftWallSubTiles[TOP_LEFT] = {0, 20};
    leftWallSubTiles[TOP_RIGHT] = {5, 20};
    leftWallSubTiles[BOTTOM_LEFT] = {0, 20};
    leftWallSubTiles[BOTTOM_RIGHT] =  {5, 21};
    
    // 우측 벽 (바닥 오른쪽) - 좌측은 우측 벽 타일, 우측은 일반 벽
    rightWallSubTiles[TOP_LEFT] = {24, 18};
    rightWallSubTiles[TOP_RIGHT] = {0, 20};
    rightWallSubTiles[BOTTOM_LEFT] = {24, 19};
    rightWallSubTiles[BOTTOM_RIGHT] ={0, 20};
    
    // 좌상단 모서리 (바닥 오른쪽 아래) - 좌상단은 일반 벽, 우상단은 좌측 벽, 좌하단은 상단 벽, 우하단은 모서리
    cornerTLSubTiles[TOP_LEFT] = {20, 19};
    cornerTLSubTiles[TOP_RIGHT] = {0, 20};
    cornerTLSubTiles[BOTTOM_LEFT] = {0, 20};
    cornerTLSubTiles[BOTTOM_RIGHT] =  {0, 20};
    
    // 우상단 모서리 (바닥 왼쪽 아래) - 좌상단은 우측 벽, 우상단은 일반 벽, 좌하단은 모서리, 우하단은 상단 벽
    cornerTRSubTiles[TOP_LEFT] = {0, 20};
    cornerTRSubTiles[TOP_RIGHT] = {19, 19};
    cornerTRSubTiles[BOTTOM_LEFT] = {0, 20};
    cornerTRSubTiles[BOTTOM_RIGHT] =  {0, 20};
    
    // 좌하단 모서리 (바닥 오른쪽 위) - 좌상단은 하단 벽, 우상단은 모서리, 좌하단은 일반 벽, 우하단은 좌측 벽
    cornerBLSubTiles[TOP_LEFT] = {24, 18};
    cornerBLSubTiles[TOP_RIGHT] = {0, 20};
    cornerBLSubTiles[BOTTOM_LEFT] = {24, 19};
    cornerBLSubTiles[BOTTOM_RIGHT] ={0, 20};

    
    // 우하단 모서리 (바닥 왼쪽 위) - 좌상단은 모서리, 우상단은 하단 벽, 좌하단은 우측 벽, 우하단은 일반 벽
    cornerBRSubTiles[TOP_LEFT] = {0, 20};
    cornerBRSubTiles[TOP_RIGHT] = {5, 20};
    cornerBRSubTiles[BOTTOM_LEFT] = {0, 20};
    cornerBRSubTiles[BOTTOM_RIGHT] =  {5, 21};
    
    // 내부 좌상단 모서리 - 세 방향이 바닥인 경우
    innerCornerTLSubTiles[TOP_LEFT] = {4, 25};
    innerCornerTLSubTiles[TOP_RIGHT] = {1, 25};
    innerCornerTLSubTiles[BOTTOM_LEFT] = {24, 19};
    innerCornerTLSubTiles[BOTTOM_RIGHT] = {0, 20};

    // 내부 우상단 모서리 - 세 방향이 바닥인 경우
    innerCornerTRSubTiles[TOP_LEFT] = {1, 25};
    innerCornerTRSubTiles[TOP_RIGHT] = {3, 25};
    innerCornerTRSubTiles[BOTTOM_LEFT] = {0, 20};
    innerCornerTRSubTiles[BOTTOM_RIGHT] = {5, 21};

    innerCornerBLSubTiles[TOP_LEFT] = {0, 12};
    innerCornerBLSubTiles[TOP_RIGHT] = {1, 12};
    innerCornerBLSubTiles[BOTTOM_LEFT] = {0, 13};
    innerCornerBLSubTiles[BOTTOM_RIGHT]  = {1, 13};
    
    innerCornerBRSubTiles[TOP_LEFT] = {0, 12};
    innerCornerBRSubTiles[TOP_RIGHT] = {1, 12};
    innerCornerBRSubTiles[BOTTOM_LEFT] = {0, 13};
    innerCornerBRSubTiles[BOTTOM_RIGHT]  = {1, 13};

    horizontalSubTiles[TOP_LEFT] = {0, 8};
    horizontalSubTiles[TOP_RIGHT] = {1, 8};
    horizontalSubTiles[BOTTOM_LEFT] = {0, 9};
    horizontalSubTiles[BOTTOM_RIGHT] = {1, 9};

    verticalSubTiles[TOP_LEFT] = {0, 8};
    verticalSubTiles[TOP_RIGHT] = {1, 8};
    verticalSubTiles[BOTTOM_LEFT] = {0, 9};
    verticalSubTiles[BOTTOM_RIGHT] = {1, 9};

    isolatedSubTiles[TOP_LEFT] = {4, 25};
    isolatedSubTiles[TOP_RIGHT] = {3, 25};
    isolatedSubTiles[BOTTOM_LEFT] = {0, 13};
    isolatedSubTiles[BOTTOM_RIGHT] = {1, 13};
    //
    // // 상단 벽 (바닥 위) - 상단은 일반 벽, 하단은 상단 벽 타일
    // topWallSubTiles[TOP_LEFT] = {0, 12};
    // topWallSubTiles[TOP_RIGHT] = {1, 12};
    // topWallSubTiles[BOTTOM_LEFT] = {0, 13};
    // topWallSubTiles[BOTTOM_RIGHT] = {1, 13};
    //
    // // 하단 벽 (바닥 아래) - 상단은 하단 벽 타일, 하단은 일반 벽
    // bottomWallSubTiles[TOP_LEFT] = {0, 25};
    // bottomWallSubTiles[TOP_RIGHT] = {1, 25};
    // bottomWallSubTiles[BOTTOM_LEFT] = {0, 20};
    // bottomWallSubTiles[BOTTOM_RIGHT] = {1, 20};
}

bool WallTileRenderer::IsWall(const std::vector<std::vector<int>>& map, int x, int y) {
    if (x < 0 || x >= map[0].size() || y < 0 || y >= map.size()) {
        return true; // 맵 경계 밖은 벽으로 처리
    }
    return map[y][x] == 0 || map[y][x] == 6; // TILE_WALL
}

bool WallTileRenderer::IsFloor(const std::vector<std::vector<int>>& map, int x, int y) {
    if (x < 0 || x >= map[0].size() || y < 0 || y >= map.size()) {
        return false; // 맵 경계 밖은 바닥이 아님
    }
    return map[y][x] == 1 || // TILE_FLOOR
           map[y][x] == 2 || // TILE_DOOR
           map[y][x] == 3 || // TILE_ENTRANCE
           map[y][x] == 4 || // TILE_EXIT
           map[y][x] == 5;   // TILE_HIDDEN_DOOR
}

WallTileRenderer::WallType WallTileRenderer::DetermineWallType(const std::vector<std::vector<int>>& map, int x, int y) {
    // 주변 타일 상태 확인 (바닥인지 아닌지)
    bool above = IsFloor(map, x, y - 1);
    bool below = IsFloor(map, x, y + 1);
    bool left = IsFloor(map, x - 1, y);
    bool right = IsFloor(map, x + 1, y);

    bool tl = IsFloor(map, x - 1, y - 1);
    bool tr = IsFloor(map, x + 1, y - 1);
    bool bl = IsFloor(map, x - 1, y + 1);
    bool br = IsFloor(map, x + 1, y + 1);

    // **벽 타입 결정 로직 개선:**
    // 가장 특수한 경우부터 체크하여 우선순위를 높입니다.

    int floor_cardinal_count = (int)above + (int)below + (int)left + (int)right;

    // 1. 고립된 벽 (주변 4방향 모두 바닥) - 필요하다면 추가
    if (floor_cardinal_count == 4) return WALL_ISOLATED;

    // 2. 직선 벽 조각 (양옆 또는 위아래가 바닥) - 필요하다면 추가
    if (left && right && !above && !below) return WALL_HORIZONTAL;
    if (above && below && !left && !right) return WALL_VERTICAL;


    // 3. 내부 모서리 벽 (벽 덩어리 안에 바닥이 파고든 형태 - 3방향이 바닥인 경우)
    // 바닥이 위, 왼쪽, 좌상단 대각선에 있는 경우 (WALL_INNER_CORNER_TL)
    if (above && left && tl) return WALL_INNER_CORNER_TL;
    // 바닥이 위, 오른쪽, 우상단 대각선에 있는 경우 (WALL_INNER_CORNER_TR)
    if (above && right && tr) return WALL_INNER_CORNER_TR;
    // 바닥이 아래, 왼쪽, 좌하단 대각선에 있는 경우 (WALL_INNER_CORNER_BL)
    if (below && left && bl) return WALL_INNER_CORNER_BL;
    // 바닥이 아래, 오른쪽, 우하단 대각선에 있는 경우 (WALL_INNER_CORNER_BR)
    if (below && right && br) return WALL_INNER_CORNER_BR;

    // 4. 외부 모서리 벽 (벽 덩어리의 모서리 형태 - 2방향은 벽/경계, 1방향 대각선은 바닥)
    // 바닥이 우하단 대각선에 있고, 아래/오른쪽은 벽/경계인 경우 (WALL_CORNER_BR)
    if (!below && !right && br) return WALL_CORNER_BR;
    // 바닥이 좌하단 대각선에 있고, 아래/왼쪽은 벽/경계인 경우 (WALL_CORNER_BL)
    if (!below && !left && bl) return WALL_CORNER_BL;
    // 바닥이 우상단 대각선에 있고, 위/오른쪽은 벽/경계인 경우 (WALL_CORNER_TR)
    if (!above && !right && tr) return WALL_CORNER_TR;
    // 바닥이 좌상단 대각선에 있고, 위/왼쪽은 벽/경계인 경우 (WALL_CORNER_TL)
    if (!above && !left && tl) return WALL_CORNER_TL;

     // 5. 직선 벽 (한 방향만 바닥)
    // 아래에만 바닥이 있는 경우 (WALL_TOP)
    if (below && floor_cardinal_count == 1) return WALL_TOP;
    // 위에만 바닥이 있는 경우 (WALL_BOTTOM)
    if (above && floor_cardinal_count == 1) return WALL_BOTTOM;
    // 오른쪽에만 바닥이 있는 경우 (WALL_LEFT)
    if (right && floor_cardinal_count == 1) return WALL_LEFT;
    // 왼쪽에만 바닥이 있는 경우 (WALL_RIGHT)
    if (left && floor_cardinal_count == 1) return WALL_RIGHT;


    // 6. 기본 벽 (주변이 모두 벽 또는 경계)
    return WALL_STANDARD;

    // 참고: Shattered Pixel Dungeon은 이보다 더 다양한 조합(예: T-junction 형태 등)을
    //       처리하기 위해 주변 8방향의 상태를 비트마스크 등으로 표현하여
    //       타일 타입을 결정하는 lookup table 방식을 사용하기도 합니다.
}

std::array<WallTileRenderer::SubTile, 4> WallTileRenderer::GetSubTiles(WallType wallType) {
    std::array<SubTile, 4> result;
    std::array<TilePosition, 4>* sourceArray = nullptr;
    
    // 벽 타일 유형에 따라 적절한 부분 타일 배열 선택
    switch (wallType) {
        case WALL_STANDARD:
            sourceArray = &standardWallSubTiles;
            break;
        case WALL_TOP:
            sourceArray = &topWallSubTiles;
            break;
        case WALL_BOTTOM:
            sourceArray = &bottomWallSubTiles;
            break;
        case WALL_LEFT:
            sourceArray = &leftWallSubTiles;
            break;
        case WALL_RIGHT:
            sourceArray = &rightWallSubTiles;
            break;
        case WALL_CORNER_TL:
            sourceArray = &cornerTLSubTiles;
            break;
        case WALL_CORNER_TR:
            sourceArray = &cornerTRSubTiles;
            break;
        case WALL_CORNER_BL:
            sourceArray = &cornerBLSubTiles;
            break;
        case WALL_CORNER_BR:
            sourceArray = &cornerBRSubTiles;
            break;
        case WALL_INNER_CORNER_TL:
            sourceArray = &innerCornerTLSubTiles;
            break;
        case WALL_INNER_CORNER_TR:
            sourceArray = &innerCornerTRSubTiles;
            break;
        case WALL_INNER_CORNER_BL:
            sourceArray = &innerCornerBLSubTiles;
            break;
        case WALL_INNER_CORNER_BR:
            sourceArray = &innerCornerBRSubTiles;
            break;
        case WALL_HORIZONTAL:
            sourceArray = &horizontalSubTiles;
            break;
        case WALL_VERTICAL:
            sourceArray = &verticalSubTiles;
            break;
        case WALL_ISOLATED:
            sourceArray = &isolatedSubTiles;
            break;
    }
    
    // 부분 타일 정보 생성
    for (int i = 0; i < 4; ++i) {
        result[i].source = (*sourceArray)[i];
        
        // 8x8 그리드 내에서의 오프셋 계산
        if (i == TOP_LEFT) {
            result[i].offsetX = 0;
            result[i].offsetY = 0;
        } else if (i == TOP_RIGHT) {
            result[i].offsetX = 8;
            result[i].offsetY = 0;
        } else if (i == BOTTOM_LEFT) {
            result[i].offsetX = 0;
            result[i].offsetY = 8;
        } else { // BOTTOM_RIGHT
            result[i].offsetX = 8;
            result[i].offsetY = 8;
        }
    }
    
    return result;
}

WallTileRenderer::WallRenderInfo WallTileRenderer::GetWallRenderInfo(const std::vector<std::vector<int>>& map, int x, int y) {
    WallRenderInfo result;
    
    // 벽 타일 유형 결정
    WallType wallType = DetermineWallType(map, x, y);
    
    // 타일 유형 설정 (enum 값이 이미 기존 시스템의 타일 타입 상수와 일치하도록 설정됨)
    result.tileType = static_cast<int>(wallType);
    
    // 부분 타일 배열 생성
    result.subTiles = GetSubTiles(wallType);
    
    return result;
}
