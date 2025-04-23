#pragma once
#include <map>

namespace Tile4x4 {
    // 코너 타일
    static constexpr int CORNER_TL = 0;
    static constexpr int CORNER_TR = 1;
    static constexpr int CORNER_BL = 2;
    static constexpr int CORNER_BR = 3;
    
    // 엣지 타일
    static constexpr int EDGE_TOP = 4;
    static constexpr int EDGE_RIGHT = 5;
    static constexpr int EDGE_BOTTOM = 6;
    static constexpr int EDGE_LEFT = 7;
    
    // 내부 타일
    static constexpr int INNER = 8;
    
    // 특수 타일
    static constexpr int DECO_1 = 9;
    static constexpr int DECO_2 = 10;
    // ...
}

// 4x4 타일 조합을 나타내는 구조체
struct TileQuad {
    int topLeft;     // 좌상단 4x4 타일
    int topRight;    // 우상단 4x4 타일
    int bottomLeft;  // 좌하단 4x4 타일
    int bottomRight; // 우하단 4x4 타일
};

// 비트마스크와 4x4 타일 조합 매핑 테이블
std::map<unsigned char, TileQuad> bitmaskToTileQuad;

void InitializeTileQuadMappings() {
    // 내부 벽 (모든 방향이 벽)
    bitmaskToTileQuad[0xFF] = {
        Tile4x4::INNER, Tile4x4::INNER, 
        Tile4x4::INNER, Tile4x4::INNER
    };
    
    // 상단 벽 (위쪽이 바닥)
    bitmaskToTileQuad[0xFE] = {
        Tile4x4::EDGE_TOP, Tile4x4::EDGE_TOP, 
        Tile4x4::INNER, Tile4x4::INNER
    };
    
    // 우측 벽 (오른쪽이 바닥)
    bitmaskToTileQuad[0xFB] = {
        Tile4x4::INNER, Tile4x4::EDGE_RIGHT, 
        Tile4x4::INNER, Tile4x4::EDGE_RIGHT
    };
    
    // 하단 벽 (아래쪽이 바닥)
    bitmaskToTileQuad[0xEF] = {
        Tile4x4::INNER, Tile4x4::INNER, 
        Tile4x4::EDGE_BOTTOM, Tile4x4::EDGE_BOTTOM
    };
    
    // 좌측 벽 (왼쪽이 바닥)
    bitmaskToTileQuad[0xBF] = {
        Tile4x4::EDGE_LEFT, Tile4x4::INNER, 
        Tile4x4::EDGE_LEFT, Tile4x4::INNER
    };
    
    // 좌상단 모서리 (왼쪽과 위쪽이 바닥)
    bitmaskToTileQuad[0xBE] = {
        Tile4x4::CORNER_TL, Tile4x4::EDGE_TOP, 
        Tile4x4::EDGE_LEFT, Tile4x4::INNER
    };
    
    // 우상단 모서리 (오른쪽과 위쪽이 바닥)
    bitmaskToTileQuad[0xFA] = {
        Tile4x4::EDGE_TOP, Tile4x4::CORNER_TR, 
        Tile4x4::INNER, Tile4x4::EDGE_RIGHT
    };
    
    // 좌하단 모서리 (왼쪽과 아래쪽이 바닥)
    bitmaskToTileQuad[0xAF] = {
        Tile4x4::EDGE_LEFT, Tile4x4::INNER, 
        Tile4x4::CORNER_BL, Tile4x4::EDGE_BOTTOM
    };
    
    // 우하단 모서리 (오른쪽과 아래쪽이 바닥)
    bitmaskToTileQuad[0xEB] = {
        Tile4x4::INNER, Tile4x4::EDGE_RIGHT, 
        Tile4x4::EDGE_BOTTOM, Tile4x4::CORNER_BR
    };
    
    // 추가적인 패턴들...
}

unsigned char CalculateBitmask(const std::vector<std::vector<int>>& map, int x, int y) {
    // 8방향 오프셋 (시계 방향으로 좌상, 상, 우상, 우, 우하, 하, 좌하, 좌)
    const int dx[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    const int dy[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
    
    unsigned char bitmask = 0;
    
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        // 맵 범위 확인
        if (nx < 0 || nx >= map[0].size() || ny < 0 || ny >= map.size()) {
            // 맵 밖은 벽으로 처리
            bitmask |= (1 << i);
            continue;
        }
        
        // 벽 타일 확인
        if (IsWall(map[ny][nx])) {
            bitmask |= (1 << i);
        }
    }
    
    return bitmask;
}

TileQuad GetTileQuadFromBitmask(unsigned char bitmask) {
    // 매핑 테이블에 해당 비트마스크가 있는지 확인
    if (bitmaskToTileQuad.find(bitmask) != bitmaskToTileQuad.end()) {
        return bitmaskToTileQuad[bitmask];
    }
    
    // 기본값 반환 (내부 타일)
    return {
        Tile4x4::INNER, Tile4x4::INNER, 
        Tile4x4::INNER, Tile4x4::INNER
    };
}

void RenderTileQuad(int x, int y, unsigned char bitmask) {
    // 비트마스크에 따라 4x4 타일 조합 가져오기
    TileQuad quad = GetTileQuadFromBitmask(bitmask);
    
    // 4x4 타일 크기 (기본 타일 크기의 1/2)
    int tileSize = TILE_SIZE / 2;
    
    // 4x4 타일 렌더링
    RenderTile(quad.topLeft, x, y);
    RenderTile(quad.topRight, x + tileSize, y);
    RenderTile(quad.bottomLeft, x, y + tileSize);
    RenderTile(quad.bottomRight, x + tileSize, y + tileSize);
}
