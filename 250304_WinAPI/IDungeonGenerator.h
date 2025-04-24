#pragma once
#include <vector>

class IDungeonGenerator {
public:
    virtual ~IDungeonGenerator() = default;

    // 던전 생성 메서드
    virtual std::vector<std::vector<int>> Generate(int width, int height) = 0;

    // 타일 변형 적용 메서드
    virtual std::vector<std::vector<int>> GetTileVariations(std::vector<std::vector<int>>& map) = 0;

    // 타일 상수
    static constexpr int TILE_NONE = 0;
    static constexpr int TILE_WALL = 1;
    static constexpr int TILE_FLOOR = 2;
    static constexpr int TILE_DOOR = 3;
    static constexpr int TILE_ENTRANCE = 4;
    static constexpr int TILE_EXIT = 5;
    static constexpr int TILE_HIDDEN_DOOR = 6;
    
    // 벽 변형 타일
    static constexpr int TILE_WALL_TOP = 7;
    static constexpr int TILE_WALL_BOTTOM = 8;
    static constexpr int TILE_WALL_LEFT = 9;
    static constexpr int TILE_WALL_RIGHT = 10;
    static constexpr int TILE_WALL_CORNER_TL = 11;  // Top-Left
    static constexpr int TILE_WALL_CORNER_TR = 12;  // Top-Right
    static constexpr int TILE_WALL_CORNER_BL = 13;  // Bottom-Left
    static constexpr int TILE_WALL_CORNER_BR = 14;  // Bottom-Right
    
    // 바닥 변형 타일
    static constexpr int TILE_FLOOR_NORMAL = 15;
    static constexpr int TILE_FLOOR_FANCY = 16;
    static constexpr int TILE_FLOOR_CRACKED = 17;
    static constexpr int TILE_FLOOR_MOSSY = 18;
}; 