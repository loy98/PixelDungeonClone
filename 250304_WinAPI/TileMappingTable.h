#pragma once
#include <map>
#include <vector>
#include "IDungeonGenerator.h"

// 타일 매핑 테이블 구현
// 비트마스크 값에 따른 타일 ID 매핑

class TileMappingTable {
public:
    TileMappingTable() {
        InitializeWallMappings();
    }

    // 비트마스크에 따른 벽 타일 ID 조회
    int GetWallTileID(unsigned char bitmask) {
        // 매핑 테이블에 해당 비트마스크가 있는지 확인
        if (wallMappings.find(bitmask) != wallMappings.end()) {
            return wallMappings[bitmask];
        }
        
        // 기본 벽 타일 반환
        return IDungeonGenerator::TILE_WALL;
    }

private:
    // 비트마스크 -> 타일 ID 매핑 테이블
    std::map<unsigned char, int> wallMappings;
    
    // 벽 타일 매핑 초기화
    void InitializeWallMappings() {
        // 모든 방향이 벽인 경우 (내부 벽)
        wallMappings[255] = IDungeonGenerator::TILE_WALL_INNER;
        
        // 독립된 벽 (주변에 벽이 없는 경우)
        wallMappings[0] = IDungeonGenerator::TILE_WALL_SOLO;
        
        // 상단 벽 (북쪽에만 바닥이 있는 경우)
        wallMappings[254] = IDungeonGenerator::TILE_WALL_TOP;
        
        // 하단 벽 (남쪽에만 바닥이 있는 경우)
        wallMappings[239] = IDungeonGenerator::TILE_WALL_BOTTOM;
        
        // 좌측 벽 (서쪽에만 바닥이 있는 경우)
        wallMappings[191] = IDungeonGenerator::TILE_WALL_LEFT;
        
        // 우측 벽 (동쪽에만 바닥이 있는 경우)
        wallMappings[251] = IDungeonGenerator::TILE_WALL_RIGHT;
        
        // 상단 좌측 모서리 (북쪽과 서쪽에 바닥이 있는 경우)
        wallMappings[190] = IDungeonGenerator::TILE_WALL_TOP_LEFT;
        
        // 상단 우측 모서리 (북쪽과 동쪽에 바닥이 있는 경우)
        wallMappings[250] = IDungeonGenerator::TILE_WALL_TOP_RIGHT;
        
        // 하단 좌측 모서리 (남쪽과 서쪽에 바닥이 있는 경우)
        wallMappings[175] = IDungeonGenerator::TILE_WALL_BOTTOM_LEFT;
        
        // 하단 우측 모서리 (남쪽과 동쪽에 바닥이 있는 경우)
        wallMappings[235] = IDungeonGenerator::TILE_WALL_BOTTOM_RIGHT;
        
        // 상단 좌측 코너 (북서쪽 코너)
        wallMappings[28] = IDungeonGenerator::TILE_WALL_CORNER_TL;
        
        // 상단 우측 코너 (북동쪽 코너)
        wallMappings[112] = IDungeonGenerator::TILE_WALL_CORNER_TR;
        
        // 하단 좌측 코너 (남서쪽 코너)
        wallMappings[7] = IDungeonGenerator::TILE_WALL_CORNER_BL;
        
        // 하단 우측 코너 (남동쪽 코너)
        wallMappings[193] = IDungeonGenerator::TILE_WALL_CORNER_BR;
        
        // 측면 상단 (북쪽과 남쪽에 벽이 없는 경우)
        wallMappings[68] = IDungeonGenerator::TILE_WALL_SIDE_TOP;
        
        // 측면 하단 (동쪽과 서쪽에 벽이 없는 경우)
        wallMappings[17] = IDungeonGenerator::TILE_WALL_SIDE_BOTTOM;
        
        // 측면 좌측 (서쪽에 벽이 없고 다른 방향에 벽이 있는 경우)
        wallMappings[17] = IDungeonGenerator::TILE_WALL_SIDE_LEFT;
        
        // 측면 우측 (동쪽에 벽이 없고 다른 방향에 벽이 있는 경우)
        wallMappings[68] = IDungeonGenerator::TILE_WALL_SIDE_RIGHT;
        
        // 추가적인 패턴들...
        // 실제 게임에서는 더 많은 패턴을 추가할 수 있습니다.
    }
};
