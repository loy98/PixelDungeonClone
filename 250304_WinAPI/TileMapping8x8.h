#pragma once
#include <map>
#include <vector>
#include "config.h"
#include "IDungeonGenerator.h"

// 8x8 타일 ID 상수 정의
namespace Tile8x8 {
    // 기본 타일: 0-99
    static constexpr int EMPTY = 0;
    static constexpr int FLOOR_BASE = 1;
    static constexpr int WALL_BASE = 10;
    
    // 벽 테두리 타일: 100-199
    static constexpr int WALL_EDGE_N = 100;  // 북쪽 테두리
    static constexpr int WALL_EDGE_E = 101;  // 동쪽 테두리
    static constexpr int WALL_EDGE_S = 102;  // 남쪽 테두리
    static constexpr int WALL_EDGE_W = 103;  // 서쪽 테두리
    
    static constexpr int WALL_CORNER_NE = 110;  // 북동쪽 코너
    static constexpr int WALL_CORNER_SE = 111;  // 남동쪽 코너
    static constexpr int WALL_CORNER_SW = 112;  // 남서쪽 코너
    static constexpr int WALL_CORNER_NW = 113;  // 북서쪽 코너
    
    static constexpr int WALL_INNER_CORNER_NE = 120;  // 북동쪽 내부 코너
    static constexpr int WALL_INNER_CORNER_SE = 121;  // 남동쪽 내부 코너
    static constexpr int WALL_INNER_CORNER_SW = 122;  // 남서쪽 내부 코너
    static constexpr int WALL_INNER_CORNER_NW = 123;  // 북서쪽 내부 코너
    
    // 바닥 테두리 타일: 200-299
    static constexpr int FLOOR_EDGE_N = 200;  // 북쪽 테두리
    static constexpr int FLOOR_EDGE_E = 201;  // 동쪽 테두리
    static constexpr int FLOOR_EDGE_S = 202;  // 남쪽 테두리
    static constexpr int FLOOR_EDGE_W = 203;  // 서쪽 테두리
    
    static constexpr int FLOOR_CORNER_NE = 210;  // 북동쪽 코너
    static constexpr int FLOOR_CORNER_SE = 211;  // 남동쪽 코너
    static constexpr int FLOOR_CORNER_SW = 212;  // 남서쪽 코너
    static constexpr int FLOOR_CORNER_NW = 213;  // 북서쪽 코너
    
    // 특수 타일: 300-399
    static constexpr int DOOR_CLOSED = 300;
    static constexpr int DOOR_OPEN = 301;
    static constexpr int ENTRANCE = 310;
    static constexpr int EXIT = 311;
    static constexpr int HIDDEN_DOOR = 320;
}

// 8x8 타일 매핑 관리 클래스 (TILE_SIZE=90 지원)
class TileMapping8x8 {
public:
    TileMapping8x8() {
        InitializeImagePositions();
    }
    
    // 타일 ID에 따른 이미지 위치 조회
    POINT GetImagePosition(int tileID) {
        // 매핑 테이블에 해당 타일 ID가 있는지 확인
        if (imagePositions.find(tileID) != imagePositions.end()) {
            return imagePositions[tileID];
        }
        
        // 기본 이미지 위치 반환
        return {0, 0}; // 기본 타일 (빈 공간)
    }
    
    // 16x16 타일을 4개의 8x8 타일로 분할
    std::vector<int> Split16x16Tile(int tileID) {
        std::vector<int> result(4, Tile8x8::EMPTY);
        
        // 타일 ID에 따라 적절한 8x8 타일 4개 반환
        switch (tileID) {
            case IDungeonGenerator::TILE_WALL:
                result[0] = Tile8x8::WALL_BASE;
                result[1] = Tile8x8::WALL_BASE;
                result[2] = Tile8x8::WALL_BASE;
                result[3] = Tile8x8::WALL_BASE;
                break;
                
            case IDungeonGenerator::TILE_FLOOR:
                result[0] = Tile8x8::FLOOR_BASE;
                result[1] = Tile8x8::FLOOR_BASE;
                result[2] = Tile8x8::FLOOR_BASE;
                result[3] = Tile8x8::FLOOR_BASE;
                break;
                
            case IDungeonGenerator::TILE_WALL_TOP:
                result[0] = Tile8x8::FLOOR_EDGE_S;
                result[1] = Tile8x8::FLOOR_EDGE_S;
                result[2] = Tile8x8::WALL_EDGE_N;
                result[3] = Tile8x8::WALL_EDGE_N;
                break;
                
            case IDungeonGenerator::TILE_WALL_BOTTOM:
                result[0] = Tile8x8::WALL_EDGE_S;
                result[1] = Tile8x8::WALL_EDGE_S;
                result[2] = Tile8x8::FLOOR_EDGE_N;
                result[3] = Tile8x8::FLOOR_EDGE_N;
                break;
                
            case IDungeonGenerator::TILE_WALL_LEFT:
                result[0] = Tile8x8::FLOOR_EDGE_E;
                result[1] = Tile8x8::WALL_EDGE_W;
                result[2] = Tile8x8::FLOOR_EDGE_E;
                result[3] = Tile8x8::WALL_EDGE_W;
                break;
                
            case IDungeonGenerator::TILE_WALL_RIGHT:
                result[0] = Tile8x8::WALL_EDGE_E;
                result[1] = Tile8x8::FLOOR_EDGE_W;
                result[2] = Tile8x8::WALL_EDGE_E;
                result[3] = Tile8x8::FLOOR_EDGE_W;
                break;
                
            case IDungeonGenerator::TILE_WALL_TOP_LEFT:
                result[0] = Tile8x8::FLOOR_CORNER_SE;
                result[1] = Tile8x8::FLOOR_EDGE_S;
                result[2] = Tile8x8::FLOOR_EDGE_E;
                result[3] = Tile8x8::WALL_INNER_CORNER_NW;
                break;
                
            case IDungeonGenerator::TILE_WALL_TOP_RIGHT:
                result[0] = Tile8x8::FLOOR_EDGE_S;
                result[1] = Tile8x8::FLOOR_CORNER_SW;
                result[2] = Tile8x8::WALL_INNER_CORNER_NE;
                result[3] = Tile8x8::FLOOR_EDGE_W;
                break;
                
            case IDungeonGenerator::TILE_WALL_BOTTOM_LEFT:
                result[0] = Tile8x8::FLOOR_EDGE_E;
                result[1] = Tile8x8::WALL_INNER_CORNER_SW;
                result[2] = Tile8x8::FLOOR_CORNER_NE;
                result[3] = Tile8x8::FLOOR_EDGE_N;
                break;
                
            case IDungeonGenerator::TILE_WALL_BOTTOM_RIGHT:
                result[0] = Tile8x8::WALL_INNER_CORNER_SE;
                result[1] = Tile8x8::FLOOR_EDGE_W;
                result[2] = Tile8x8::FLOOR_EDGE_N;
                result[3] = Tile8x8::FLOOR_CORNER_NW;
                break;
                
            case IDungeonGenerator::TILE_WALL_CORNER_TL:
                result[0] = Tile8x8::WALL_BASE;
                result[1] = Tile8x8::WALL_EDGE_W;
                result[2] = Tile8x8::WALL_EDGE_N;
                result[3] = Tile8x8::WALL_CORNER_NW;
                break;
                
            case IDungeonGenerator::TILE_WALL_CORNER_TR:
                result[0] = Tile8x8::WALL_EDGE_E;
                result[1] = Tile8x8::WALL_BASE;
                result[2] = Tile8x8::WALL_CORNER_NE;
                result[3] = Tile8x8::WALL_EDGE_N;
                break;
                
            case IDungeonGenerator::TILE_WALL_CORNER_BL:
                result[0] = Tile8x8::WALL_EDGE_S;
                result[1] = Tile8x8::WALL_CORNER_SW;
                result[2] = Tile8x8::WALL_BASE;
                result[3] = Tile8x8::WALL_EDGE_W;
                break;
                
            case IDungeonGenerator::TILE_WALL_CORNER_BR:
                result[0] = Tile8x8::WALL_CORNER_SE;
                result[1] = Tile8x8::WALL_EDGE_S;
                result[2] = Tile8x8::WALL_EDGE_E;
                result[3] = Tile8x8::WALL_BASE;
                break;
                
            case IDungeonGenerator::TILE_WALL_INNER:
                result[0] = Tile8x8::WALL_BASE;
                result[1] = Tile8x8::WALL_BASE;
                result[2] = Tile8x8::WALL_BASE;
                result[3] = Tile8x8::WALL_BASE;
                break;
                
            case IDungeonGenerator::TILE_WALL_SOLO:
                result[0] = Tile8x8::WALL_CORNER_NW;
                result[1] = Tile8x8::WALL_CORNER_NE;
                result[2] = Tile8x8::WALL_CORNER_SW;
                result[3] = Tile8x8::WALL_CORNER_SE;
                break;
                
            case IDungeonGenerator::TILE_DOOR:
                result[0] = Tile8x8::DOOR_CLOSED;
                result[1] = Tile8x8::DOOR_CLOSED;
                result[2] = Tile8x8::DOOR_CLOSED;
                result[3] = Tile8x8::DOOR_CLOSED;
                break;
                
            case IDungeonGenerator::TILE_ENTRANCE:
                result[0] = Tile8x8::ENTRANCE;
                result[1] = Tile8x8::ENTRANCE;
                result[2] = Tile8x8::ENTRANCE;
                result[3] = Tile8x8::ENTRANCE;
                break;
                
            case IDungeonGenerator::TILE_EXIT:
                result[0] = Tile8x8::EXIT;
                result[1] = Tile8x8::EXIT;
                result[2] = Tile8x8::EXIT;
                result[3] = Tile8x8::EXIT;
                break;
                
            case IDungeonGenerator::TILE_HIDDEN_DOOR:
                result[0] = Tile8x8::HIDDEN_DOOR;
                result[1] = Tile8x8::HIDDEN_DOOR;
                result[2] = Tile8x8::HIDDEN_DOOR;
                result[3] = Tile8x8::HIDDEN_DOOR;
                break;
                
            default:
                // 기본값은 빈 공간
                result[0] = Tile8x8::EMPTY;
                result[1] = Tile8x8::EMPTY;
                result[2] = Tile8x8::EMPTY;
                result[3] = Tile8x8::EMPTY;
                break;
        }
        
        return result;
    }
    
    // 주변 타일에 따라 테두리 타일 결정
    int DetermineEdgeTile(const std::vector<std::vector<int>>& map, int x, int y, int quadrant) {
        // 맵 크기 확인
        int height = map.size();
        int width = map[0].size();
        
        // 8x8 타일 좌표 계산
        int tileX = x * 2 + (quadrant % 2);
        int tileY = y * 2 + (quadrant / 2);
        
        // 주변 타일 확인
        bool hasNorth = (tileY > 0) ? IsWall(map, tileX, tileY - 1) : true;
        bool hasSouth = (tileY < height * 2 - 1) ? IsWall(map, tileX, tileY + 1) : true;
        bool hasEast = (tileX < width * 2 - 1) ? IsWall(map, tileX + 1, tileY) : true;
        bool hasWest = (tileX > 0) ? IsWall(map, tileX - 1, tileY) : true;
        
        // 대각선 방향 확인
        bool hasNorthEast = (tileY > 0 && tileX < width * 2 - 1) ? IsWall(map, tileX + 1, tileY - 1) : true;
        bool hasSouthEast = (tileY < height * 2 - 1 && tileX < width * 2 - 1) ? IsWall(map, tileX + 1, tileY + 1) : true;
        bool hasSouthWest = (tileY < height * 2 - 1 && tileX > 0) ? IsWall(map, tileX - 1, tileY + 1) : true;
        bool hasNorthWest = (tileY > 0 && tileX > 0) ? IsWall(map, tileX - 1, tileY - 1) : true;
        
        // 현재 타일이 벽인지 확인
        bool isWall = IsWall(map, tileX, tileY);
        
        // 테두리 타일 결정
        if (isWall) {
            // 벽 테두리 타일
            if (!hasNorth) return Tile8x8::WALL_EDGE_N;
            if (!hasSouth) return Tile8x8::WALL_EDGE_S;
            if (!hasEast) return Tile8x8::WALL_EDGE_E;
            if (!hasWest) return Tile8x8::WALL_EDGE_W;
            
            // 코너 타일
            if (!hasNorthEast) return Tile8x8::WALL_CORNER_NE;
            if (!hasSouthEast) return Tile8x8::WALL_CORNER_SE;
            if (!hasSouthWest) return Tile8x8::WALL_CORNER_SW;
            if (!hasNorthWest) return Tile8x8::WALL_CORNER_NW;
            
            // 내부 코너 타일
            if (!hasNorth && !hasEast) return Tile8x8::WALL_INNER_CORNER_NE;
            if (!hasSouth && !hasEast) return Tile8x8::WALL_INNER_CORNER_SE;
            if (!hasSouth && !hasWest) return Tile8x8::WALL_INNER_CORNER_SW;
            if (!hasNorth && !hasWest) return Tile8x8::WALL_INNER_CORNER_NW;
            
            // 기본 벽 타일
            return Tile8x8::WALL_BASE;
        } else {
            // 바닥 테두리 타일
            if (hasNorth) return Tile8x8::FLOOR_EDGE_N;
            if (hasSouth) return Tile8x8::FLOOR_EDGE_S;
            if (hasEast) return Tile8x8::FLOOR_EDGE_E;
            if (hasWest) return Tile8x8::FLOOR_EDGE_W;
            
            // 코너 타일
            if (hasNorthEast) return Tile8x8::FLOOR_CORNER_NE;
            if (hasSouthEast) return Tile8x8::FLOOR_CORNER_SE;
            if (hasSouthWest) return Tile8x8::FLOOR_CORNER_SW;
            if (hasNorthWest) return Tile8x8::FLOOR_CORNER_NW;
            
            // 기본 바닥 타일
            return Tile8x8::FLOOR_BASE;
        }
    }
    
    // 16x16 맵을 8x8 맵으로 변환
    std::vector<std::vector<int>> Convert16x16MapTo8x8(const std::vector<std::vector<int>>& map16x16) {
        int height = map16x16.size();
        int width = map16x16[0].size();
        
        // 8x8 맵 생성 (크기는 2배)
        std::vector<std::vector<int>> map8x8(height * 2, std::vector<int>(width * 2, Tile8x8::EMPTY));
        
        // 각 16x16 타일을 4개의 8x8 타일로 분할
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                std::vector<int> subTiles = Split16x16Tile(map16x16[y][x]);
                
                // 4개의 8x8 타일 배치
                map8x8[y * 2][x * 2] = subTiles[0];         // 좌상단
                map8x8[y * 2][x * 2 + 1] = subTiles[1];     // 우상단
                map8x8[y * 2 + 1][x * 2] = subTiles[2];     // 좌하단
                map8x8[y * 2 + 1][x * 2 + 1] = subTiles[3]; // 우하단
            }
        }
        
        return map8x8;
    }
    
    // 테두리 타일 적용
    std::vector<std::vector<int>> ApplyEdgeTiles(const std::vector<std::vector<int>>& map16x16) {
        int height = map16x16.size();
        int width = map16x16[0].size();
        
        // 먼저 16x16 맵을 8x8 맵으로 변환
        std::vector<std::vector<int>> map8x8 = Convert16x16MapTo8x8(map16x16);
        
        // 각 8x8 타일에 대해 테두리 타일 적용
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // 각 16x16 타일의 4개 분면에 대해 테두리 타일 결정
                for (int q = 0; q < 4; q++) {
                    int tileX = x * 2 + (q % 2);
                    int tileY = y * 2 + (q / 2);
                    
                    // 테두리 타일 결정
                    map8x8[tileY][tileX] = DetermineEdgeTile(map16x16, x, y, q);
                }
            }
        }
        
        return map8x8;
    }

private:
    // 타일 ID -> 이미지 위치 매핑 테이블
    std::map<int, POINT> imagePositions;
    
    // 이미지 위치 매핑 초기화
    void InitializeImagePositions() {
        // 기본 타일
        imagePositions[Tile8x8::EMPTY] = {0, 9};  // 빈 공간
        imagePositions[Tile8x8::FLOOR_BASE] = {0, 0};  // 기본 바닥
        imagePositions[Tile8x8::WALL_BASE] = {0, 3};  // 기본 벽
        
        // 벽 테두리 타일
        imagePositions[Tile8x8::WALL_EDGE_N] = {0, 6};  // 북쪽 테두리
        imagePositions[Tile8x8::WALL_EDGE_E] = {4, 9};  // 동쪽 테두리
        imagePositions[Tile8x8::WALL_EDGE_S] = {5, 8};  // 남쪽 테두리
        imagePositions[Tile8x8::WALL_EDGE_W] = {2, 9};  // 서쪽 테두리
        
        imagePositions[Tile8x8::WALL_CORNER_NE] = {1, 13};  // 북동쪽 코너
        imagePositions[Tile8x8::WALL_CORNER_SE] = {5, 13};  // 남동쪽 코너
        imagePositions[Tile8x8::WALL_CORNER_SW] = {3, 13};  // 남서쪽 코너
        imagePositions[Tile8x8::WALL_CORNER_NW] = {9, 6};   // 북서쪽 코너
        
        imagePositions[Tile8x8::WALL_INNER_CORNER_NE] = {2, 7};  // 북동쪽 내부 코너
        imagePositions[Tile8x8::WALL_INNER_CORNER_SE] = {8, 9};  // 남동쪽 내부 코너
        imagePositions[Tile8x8::WALL_INNER_CORNER_SW] = {1, 9};  // 남서쪽 내부 코너
        imagePositions[Tile8x8::WALL_INNER_CORNER_NW] = {0, 7};  // 북서쪽 내부 코너
        
        // 바닥 테두리 타일
        imagePositions[Tile8x8::FLOOR_EDGE_N] = {0, 0};  // 북쪽 테두리
        imagePositions[Tile8x8::FLOOR_EDGE_E] = {0, 0};  // 동쪽 테두리
        imagePositions[Tile8x8::FLOOR_EDGE_S] = {0, 0};  // 남쪽 테두리
        imagePositions[Tile8x8::FLOOR_EDGE_W] = {0, 0};  // 서쪽 테두리
        
        imagePositions[Tile8x8::FLOOR_CORNER_NE] = {0, 0};  // 북동쪽 코너
        imagePositions[Tile8x8::FLOOR_CORNER_SE] = {0, 0};  // 남동쪽 코너
        imagePositions[Tile8x8::FLOOR_CORNER_SW] = {0, 0};  // 남서쪽 코너
        imagePositions[Tile8x8::FLOOR_CORNER_NW] = {0, 0};  // 북서쪽 코너
        
        // 특수 타일
        imagePositions[Tile8x8::DOOR_CLOSED] = {8, 3};  // 닫힌 문
        imagePositions[Tile8x8::DOOR_OPEN] = {9, 3};    // 열린 문
        imagePositions[Tile8x8::ENTRANCE] = {0, 1};     // 입구
        imagePositions[Tile8x8::EXIT] = {1, 1};         // 출구
        imagePositions[Tile8x8::HIDDEN_DOOR] = {13, 3}; // 숨겨진 문
    }
    
    // 타일이 벽인지 확인하는 헬퍼 함수
    bool IsWall(const std::vector<std::vector<int>>& map, int x, int y) {
        // 맵 범위 확인
        int height = map.size();
        int width = map[0].size();
        
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return true; // 맵 밖은 벽으로 처리
        }
        
        // 벽 타일 확인
        int tileID = map[y][x];
        return tileID == IDungeonGenerator::TILE_WALL ||
               tileID == IDungeonGenerator::TILE_WALL_TOP ||
               tileID == IDungeonGenerator::TILE_WALL_BOTTOM ||
               tileID == IDungeonGenerator::TILE_WALL_LEFT ||
               tileID == IDungeonGenerator::TILE_WALL_RIGHT ||
               tileID == IDungeonGenerator::TILE_WALL_CORNER_TL ||
               tileID == IDungeonGenerator::TILE_WALL_CORNER_TR ||
               tileID == IDungeonGenerator::TILE_WALL_CORNER_BL ||
               tileID == IDungeonGenerator::TILE_WALL_CORNER_BR ||
               tileID == IDungeonGenerator::TILE_WALL_INNER ||
               tileID == IDungeonGenerator::TILE_WALL_SOLO;
    }
};
