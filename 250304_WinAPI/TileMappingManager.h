#pragma once
#include <map>
#include <vector>
#include "IDungeonGenerator.h"
#include "config.h"

// 타일 매핑 관리 클래스
// 비트마스크 값에 따른 타일 ID 매핑 및 타일 ID에 따른 이미지 위치 매핑을 관리합니다.
class TileMappingManager {
public:
    TileMappingManager() {
        InitializeWallMappings();
        InitializeImagePositions();
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
    
    // 타일 ID에 따른 이미지 위치 조회
    POINT GetImagePosition(int tileID) {
        // 매핑 테이블에 해당 타일 ID가 있는지 확인
        if (imagePositions.find(tileID) != imagePositions.end()) {
            return imagePositions[tileID];
        }
        
        // 기본 이미지 위치 반환
        return {0, 4}; // 기본 타일 (알 수 없는 타일 타입)
    }

private:
    // 비트마스크 -> 타일 ID 매핑 테이블
    std::map<unsigned char, int> wallMappings;
    
    // 타일 ID -> 이미지 위치 매핑 테이블
    std::map<int, POINT> imagePositions;
    
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
    
    // 이미지 위치 매핑 초기화
    void InitializeImagePositions() {
        // 기본 타일
        imagePositions[IDungeonGenerator::TILE_WALL] = {0, 3}; // 일반 벽 타일
        imagePositions[IDungeonGenerator::TILE_FLOOR] = {0, 0}; // 일반 바닥 타일
        imagePositions[IDungeonGenerator::TILE_DOOR] = {8, 3}; // 문
        imagePositions[IDungeonGenerator::TILE_ENTRANCE] = {0, 1}; // 입구
        imagePositions[IDungeonGenerator::TILE_EXIT] = {1, 1}; // 출구
        imagePositions[IDungeonGenerator::TILE_HIDDEN_DOOR] = {13, 3}; // 숨겨진 문
        imagePositions[IDungeonGenerator::TILE_NONE] = {0, 9}; // 빈 공간
        
        // 벽 변형 타일
        imagePositions[IDungeonGenerator::TILE_WALL_TOP] = {0, 6}; // 상단 벽(위쪽이 바닥)
        imagePositions[IDungeonGenerator::TILE_WALL_BOTTOM] = {5, 8}; // 하단 벽(아래쪽이 바닥)
        imagePositions[IDungeonGenerator::TILE_WALL_LEFT] = {2, 9}; // 좌측 벽(왼쪽이 바닥)
        imagePositions[IDungeonGenerator::TILE_WALL_RIGHT] = {4, 9}; // 우측 벽(오른쪽이 바닥)
        
        // 코너 타일
        imagePositions[IDungeonGenerator::TILE_WALL_CORNER_TL] = {9, 6}; // 상단 좌측 코너(북서쪽 코너)
        imagePositions[IDungeonGenerator::TILE_WALL_CORNER_TR] = {1, 13}; // 상단 우측 코너(북동쪽 코너)
        imagePositions[IDungeonGenerator::TILE_WALL_CORNER_BL] = {3, 13}; // 하단 좌측 코너(남서쪽 코너)
        imagePositions[IDungeonGenerator::TILE_WALL_CORNER_BR] = {5, 13}; // 하단 우측 코너(남동쪽 코너)
        
        // 모서리 타일
        imagePositions[IDungeonGenerator::TILE_WALL_TOP_LEFT] = {0, 7}; // 상단 좌측 모서리(위쪽과 왼쪽이 바닥)
        imagePositions[IDungeonGenerator::TILE_WALL_TOP_RIGHT] = {2, 7}; // 상단 우측 모서리(위쪽과 오른쪽이 바닥)
        imagePositions[IDungeonGenerator::TILE_WALL_BOTTOM_LEFT] = {1, 9}; // 하단 좌측 모서리(아래쪽과 왼쪽이 바닥)
        imagePositions[IDungeonGenerator::TILE_WALL_BOTTOM_RIGHT] = {8, 9}; // 하단 우측 모서리(아래쪽과 오른쪽이 바닥)
        
        // 내부 벽 및 특수 타일
        imagePositions[IDungeonGenerator::TILE_WALL_INNER] = {1, 8}; // 내부 벽(모든 방향이 벽)
        imagePositions[IDungeonGenerator::TILE_WALL_SOLO] = {3, 8}; // 독립된 벽(주변에 벽이 없음)
        imagePositions[IDungeonGenerator::TILE_WALL_SIDE_TOP] = {6, 9}; // 측면 상단(위쪽과 아래쪽이 바닥)
        imagePositions[IDungeonGenerator::TILE_WALL_SIDE_BOTTOM] = {7, 9}; // 측면 하단(왼쪽과 오른쪽이 바닥)
        imagePositions[IDungeonGenerator::TILE_WALL_SIDE_LEFT] = {3, 9}; // 측면 좌측(왼쪽이 바닥, 위아래가 벽)
        imagePositions[IDungeonGenerator::TILE_WALL_SIDE_RIGHT] = {5, 9}; // 측면 우측(오른쪽이 바닥, 위아래가 벽)
        
        // 바닥 변형 타일
        imagePositions[IDungeonGenerator::TILE_FLOOR_NORMAL] = {0, 0}; // 일반 바닥
        imagePositions[IDungeonGenerator::TILE_FLOOR_FANCY] = {1, 0}; // 장식된 바닥
        imagePositions[IDungeonGenerator::TILE_FLOOR_CRACKED] = {2, 0}; // 금이 간 바닥
        imagePositions[IDungeonGenerator::TILE_FLOOR_MOSSY] = {6, 0}; // 이끼 낀 바닥
    }
};
