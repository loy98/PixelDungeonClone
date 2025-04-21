// DungeonGenerator.h
#pragma once
#include "config.h"

class DungeonGenerator {
public:
    DungeonGenerator();
    ~DungeonGenerator();

    // 주요 던전 생성 메서드
    std::vector<std::vector<int>> Generate(int width, int height);
    
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
        SPECIAL,    // 특수 방
        HIDDEN      // 숨겨진 방
    };

    // 확장된 방 클래스
    struct RoomNode {
        Room room;
        RoomType type;
        bool isOnMainPath;
        std::vector<int> connections; // 연결된 방 인덱스
        
        RoomNode(const Room& r, RoomType t) : room(r), type(t), isOnMainPath(false) {}
    };
    
    // 방 생성 함수
    std::vector<Room> GenerateRooms(int width, int height, int minRooms, int maxRooms);
    
    // 루프 빌더 함수 (메인 경로 + 가지)
    void BuildLoopLayout(std::vector<RoomNode>& roomNodes);
    
    // 방 연결 함수
    void ConnectRooms(std::vector<std::vector<int>>& map, const std::vector<RoomNode>& roomNodes);
    
    // 복도 생성 함수
    void CreateCorridor(std::vector<std::vector<int>>& map, int x1, int y1, int x2, int y2);
    
    // 특수 방 배치 함수
    void PlaceSpecialRooms(std::vector<std::vector<int>>& map, std::vector<RoomNode>& roomNodes);
    
    // 숨겨진 방 배치 함수
    void PlaceHiddenRooms(std::vector<std::vector<int>>& map, std::vector<RoomNode>& roomNodes);
    
    // 벽 정리 함수 (불필요한 벽 제거)
    void CleanupWalls(std::vector<std::vector<int>>& map);

    // 방과 복도 사이에 문 배치 함수
    void PlaceDoorsAtRoomBorders(std::vector<std::vector<int>>& map, const std::vector<RoomNode>& roomNodes);

    // 상수 정의
    enum TileType {
        TILE_WALL = 0,   // 벽
        TILE_FLOOR = 1,  // 바닥
        TILE_DOOR = 2,   // 문
        TILE_ENTRANCE = 3, // 입구
        TILE_EXIT = 4,   // 출구
        TILE_HIDDEN_DOOR = 5, // 숨겨진 문
        TILE_NONE = 6,
    };
    
    // 방 그리기 함수
    void DrawRoom(std::vector<std::vector<int>>& map, const Room& room, int tileType = TILE_FLOOR);
    
    // 방 크기 범위 설정
    const int MIN_ROOM_SIZE = 4;
    const int MAX_STANDARD_ROOM_SIZE = 8;
    const int MAX_LARGE_ROOM_SIZE = 12;
    const int MAX_GIANT_ROOM_SIZE = 16;
    
    // 방 사이 최소 간격
    const int ROOM_MARGIN = 2;
    
    // 랜덤 유틸리티 함수
    int GetRandomInt(int min, int max);
    bool GetRandomBool(float probability = 0.5f);
};
