// DungeonGenerator.h
#pragma once
#include "config.h"
#include <vector>

class DungeonGenerator {
public:
    DungeonGenerator();
    ~DungeonGenerator();

    // 주요 던전 생성 메서드
    std::vector<std::vector<int>> Generate(int width, int height);
    std::vector<std::vector<int>> GetTileVariations(std::vector<std::vector<int>>& map);

private:
    // 방 정보를 저장하는 구조체
    struct Room {
        int left, top, right, bottom;
        int centerX() const { return (left + right) / 2; }
        int centerY() const { return (top + bottom) / 2; }
        bool intersects(const Room& other, int margin = 1) const;
        int width() const { return right - left + 1; }
        int height() const { return bottom - top + 1; }
        int area() const { return width() * height(); }
        bool isLarge() const { return width() >= 8 && height() >= 8 && width() < 12 && height() < 12; }
        bool isGiant() const { return width() >= 12 && height() >= 12; }
    };

    // 방 유형 열거형
    enum RoomType {
        STANDARD,   // 표준 방
        ENTRANCE,   // 입구 방
        EXIT,       // 출구 방
        //SPECIAL,    // 특수 방
        //HIDDEN      // 숨겨진 방
    };

    // 확장된 방 클래스
    struct RoomNode {
        Room room;
        RoomType type;
        bool isOnMainPath;
        std::vector<int> connections; // 연결된 방 인덱스
        
        RoomNode(const Room& r, RoomType t) : room(r), type(t), isOnMainPath(false) {}
    };

    // 타일 유형 상세 정의
    enum TileType {
        // 기본 타일 (0-9)
        TILE_WALL = 0,   // 기본 벽
        TILE_FLOOR = 1,  // 기본 바닥
        TILE_DOOR = 2,   // 문
        TILE_ENTRANCE = 3, // 입구
        TILE_EXIT = 4,   // 출구
        TILE_HIDDEN_DOOR = 5, // 숨겨진 문
        TILE_NONE = 6,   // 빈 공간

        // 벽 변형 (10-19)
        TILE_WALL_TOP = 10,      // 위쪽 벽
        TILE_WALL_BOTTOM = 11,   // 아래쪽 벽
        TILE_WALL_LEFT = 12,     // 왼쪽 벽
        TILE_WALL_RIGHT = 13,    // 오른쪽 벽
        TILE_WALL_CORNER_TL = 14, // 왼쪽 위 모서리
        TILE_WALL_CORNER_TR = 15, // 오른쪽 위 모서리
        TILE_WALL_CORNER_BL = 16, // 왼쪽 아래 모서리
        TILE_WALL_CORNER_BR = 17, // 오른쪽 아래 모서리

        // 바닥 변형 (20-29)
        TILE_FLOOR_NORMAL = 20,  // 일반 바닥
        TILE_FLOOR_FANCY = 21,   // 장식된 바닥
        TILE_FLOOR_CRACKED = 22, // 금이 간 바닥
        TILE_FLOOR_MOSSY = 23,   // 이끼 낀 바닥
    };

    // 방 생성 함수
    std::vector<Room> GenerateRooms(int width, int height, int minRooms, int maxRooms);
    void BuildLoopLayout(std::vector<RoomNode>& roomNodes);
    void ConnectRooms(std::vector<std::vector<int>>& map, const std::vector<RoomNode>& roomNodes);
    void CreateCorridor(std::vector<std::vector<int>>& map, int x1, int y1, int x2, int y2);
    void PlaceSpecialRooms(std::vector<std::vector<int>>& map, std::vector<RoomNode>& roomNodes);
    // void PlaceHiddenRooms(std::vector<std::vector<int>>& map, std::vector<RoomNode>& roomNodes);
    void CleanupWalls(std::vector<std::vector<int>>& map);
    void PlaceDoorsAtRoomBorders(std::vector<std::vector<int>>& map, const std::vector<RoomNode>& roomNodes);
    void ApplyTileVariations(std::vector<std::vector<int>>& map);
    int DetermineWallVariation(const std::vector<std::vector<int>>& map, int x, int y);
    int DetermineFloorVariation(const std::vector<std::vector<int>>& map, int x, int y);
    void DrawRoom(std::vector<std::vector<int>>& map, const Room& room, int tileType = TILE_FLOOR);
    
    // 방 크기 범위 설정
    const int MIN_ROOM_SIZE = 4;
    const int MAX_STANDARD_ROOM_SIZE = 8;
    const int MAX_LARGE_ROOM_SIZE = 12;
    const int MAX_GIANT_ROOM_SIZE = 16;
    const int ROOM_MARGIN = 2;
    
    // 랜덤 유틸리티 함수
    int GetRandomInt(int min, int max);
    bool GetRandomBool(float probability = 0.5f);
};
