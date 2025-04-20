// DungeonGenerator.cpp
#define NOMINMAX
#include "DungeonGenerator.h"
#include <random>
#include <algorithm>
#include <ctime>
#include <queue>

DungeonGenerator::DungeonGenerator() {
    // 랜덤 시드 초기화
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

DungeonGenerator::~DungeonGenerator() {
}

// 두 방이 겹치는지 확인
bool DungeonGenerator::Room::intersects(const Room& other, int margin) const {
    return !(right + margin < other.left || 
             left - margin > other.right || 
             bottom + margin < other.top || 
             top - margin > other.bottom);
}

// 전체 던전 생성 메서드
std::vector<std::vector<int>> DungeonGenerator::Generate(int width, int height) {
    // 1. 맵 초기화 (모두 벽으로)
    std::vector<std::vector<int>> map(height, std::vector<int>(width, TILE_WALL));
    
    // 2. 방 생성
    std::vector<Room> rooms = GenerateRooms(width, height, 8, 15);
    
    // 방이 너무 적으면 다시 시도
    if (rooms.size() < 8) {
        return Generate(width, height);
    }
    
    // 3. 방 노드 생성
    std::vector<RoomNode> roomNodes;
    for (const auto& room : rooms) {
        roomNodes.push_back(RoomNode(room, STANDARD));
    }
    
    // 4. 루프 레이아웃 구축 (메인 경로 + 가지)
    BuildLoopLayout(roomNodes);
    
    // 5. 방 적용 (맵에 그리기)
    for (const auto& node : roomNodes) {
        DrawRoom(map, node.room);
    }
    
    // 6. 방 연결
    ConnectRooms(map, roomNodes);
    
    // 7. 특수 방 배치
    PlaceSpecialRooms(map, roomNodes);
    
    // 8. 숨겨진 방 배치
    PlaceHiddenRooms(map, roomNodes);
    
    // 9. 맵 정리
    CleanupWalls(map);
    
    return map;
}

// 방 생성 메서드
std::vector<DungeonGenerator::Room> DungeonGenerator::GenerateRooms(int width, int height, int minRooms, int maxRooms) {
    std::vector<Room> rooms;
    
    // 생성할 방 개수 결정
    int roomCount = GetRandomInt(minRooms, maxRooms);
    
    // 최대 시도 횟수 (무한 루프 방지)
    const int MAX_ATTEMPTS = 300;
    int attempts = 0;
    
    while (rooms.size() < roomCount && attempts < MAX_ATTEMPTS) {
        // 방 크기 유형 결정 (표준, 대형, 거대)
        float roomTypeRoll = static_cast<float>(rand()) / RAND_MAX;
        int minSize = MIN_ROOM_SIZE;
        int maxSize;
        
        if (roomTypeRoll < 0.7f) {
            // 70% 확률로 표준 방
            maxSize = MAX_STANDARD_ROOM_SIZE;
        } else if (roomTypeRoll < 0.9f) {
            // 20% 확률로 대형 방
            maxSize = MAX_LARGE_ROOM_SIZE;
        } else {
            // 10% 확률로 거대 방
            maxSize = MAX_GIANT_ROOM_SIZE;
        }
        
        // 랜덤 방 크기 생성
        int roomWidth = GetRandomInt(minSize, maxSize);
        int roomHeight = GetRandomInt(minSize, maxSize);
        
        // 랜덤 방 위치 생성 (맵 경계 내)
        int left = GetRandomInt(1, width - roomWidth - 1);
        int top = GetRandomInt(1, height - roomHeight - 1);
        
        Room newRoom = {
            left,                // left
            top,                 // top
            left + roomWidth - 1,    // right
            top + roomHeight - 1     // bottom
        };
        
        // 다른 방과 겹치는지 확인
        bool overlaps = false;
        for (const auto& room : rooms) {
            if (newRoom.intersects(room, ROOM_MARGIN)) {
                overlaps = true;
                break;
            }
        }
        
        // 겹치지 않으면 방 추가
        if (!overlaps) {
            rooms.push_back(newRoom);
        }
        
        attempts++;
    }
    
    return rooms;
}

// 루프 빌더 함수 (메인 경로 + 가지)
void DungeonGenerator::BuildLoopLayout(std::vector<RoomNode>& roomNodes) {
    if (roomNodes.size() < 3) return;
    
    // 방 크기에 따라 정렬 (큰 방이 먼저 오도록)
    std::sort(roomNodes.begin(), roomNodes.end(), [](const RoomNode& a, const RoomNode& b) {
        return a.room.area() > b.room.area();
    });
    
    // 가장 큰 방을 중앙 방으로 설정
    int centerIdx = 0;
    roomNodes[centerIdx].isOnMainPath = true;
    
    // 메인 루프를 위한 방 선택 (전체 방의 약 60%)
    int mainPathCount = std::max(3, static_cast<int>(roomNodes.size() * 0.6f));
    
    // 중앙 방에서 가장 멀리 떨어진 방을 입구로 설정
    int entranceIdx = -1;
    int maxDistance = -1;
    
    for (size_t i = 1; i < roomNodes.size(); i++) {
        int distance = std::abs(roomNodes[i].room.centerX() - roomNodes[centerIdx].room.centerX()) +
                       std::abs(roomNodes[i].room.centerY() - roomNodes[centerIdx].room.centerY());
        
        if (distance > maxDistance) {
            maxDistance = distance;
            entranceIdx = i;
        }
    }
    
    roomNodes[entranceIdx].type = ENTRANCE;
    roomNodes[entranceIdx].isOnMainPath = true;
    
    // 입구에서 가장 멀리 떨어진 방을 출구로 설정
    int exitIdx = -1;
    maxDistance = -1;
    
    for (size_t i = 1; i < roomNodes.size(); i++) {
        if (i == entranceIdx) continue;
        
        int distance = std::abs(roomNodes[i].room.centerX() - roomNodes[entranceIdx].room.centerX()) +
                       std::abs(roomNodes[i].room.centerY() - roomNodes[entranceIdx].room.centerY());
        
        if (distance > maxDistance) {
            maxDistance = distance;
            exitIdx = i;
        }
    }
    
    roomNodes[exitIdx].type = EXIT;
    roomNodes[exitIdx].isOnMainPath = true;
    
    // 메인 루프 경로 생성 (입구 -> 중앙 -> 출구 경로 주변으로 원형 경로)
    std::vector<int> mainPathIndices;
    mainPathIndices.push_back(entranceIdx);
    
    // 입구와 중앙 사이의 방들 선택
    std::vector<std::pair<int, int>> candidatesEntryToCenter;
    for (size_t i = 0; i < roomNodes.size(); i++) {
        if (i == entranceIdx || i == centerIdx || i == exitIdx) continue;
        
        // 입구와 중앙 사이에 위치하는지 확인 (대략적인 방향)
        bool isInDirection = false;
        
        // 입구에서 중앙으로의 방향 벡터
        int dirX = roomNodes[centerIdx].room.centerX() - roomNodes[entranceIdx].room.centerX();
        int dirY = roomNodes[centerIdx].room.centerY() - roomNodes[entranceIdx].room.centerY();
        
        // 입구에서 현재 방으로의 방향 벡터
        int roomDirX = roomNodes[i].room.centerX() - roomNodes[entranceIdx].room.centerX();
        int roomDirY = roomNodes[i].room.centerY() - roomNodes[entranceIdx].room.centerY();
        
        // 내적이 양수면 같은 방향
        if (dirX * roomDirX + dirY * roomDirY > 0) {
            isInDirection = true;
        }
        
        if (isInDirection) {
            int distToEntry = std::abs(roomNodes[i].room.centerX() - roomNodes[entranceIdx].room.centerX()) +
                             std::abs(roomNodes[i].room.centerY() - roomNodes[entranceIdx].room.centerY());
            
            candidatesEntryToCenter.push_back({i, distToEntry});
        }
    }
    
    // 거리에 따라 정렬
    std::sort(candidatesEntryToCenter.begin(), candidatesEntryToCenter.end(), 
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  return a.second < b.second;
              });
    
    // 입구와 중앙 사이에 1-2개 방 선택
    int midPointCount = std::min(2, static_cast<int>(candidatesEntryToCenter.size()));
    for (int i = 0; i < midPointCount; i++) {
        mainPathIndices.push_back(candidatesEntryToCenter[i].first);
        roomNodes[candidatesEntryToCenter[i].first].isOnMainPath = true;
    }
    
    mainPathIndices.push_back(centerIdx);
    
    // 중앙과 출구 사이의 방들 선택
    std::vector<std::pair<int, int>> candidatesCenterToExit;
    for (size_t i = 0; i < roomNodes.size(); i++) {
        if (i == entranceIdx || i == centerIdx || i == exitIdx) continue;
        if (roomNodes[i].isOnMainPath) continue; // 이미 메인 경로에 있는 방은 제외
        
        // 중앙과 출구 사이에 위치하는지 확인 (대략적인 방향)
        bool isInDirection = false;
        
        // 중앙에서 출구로의 방향 벡터
        int dirX = roomNodes[exitIdx].room.centerX() - roomNodes[centerIdx].room.centerX();
        int dirY = roomNodes[exitIdx].room.centerY() - roomNodes[centerIdx].room.centerY();
        
        // 중앙에서 현재 방으로의 방향 벡터
        int roomDirX = roomNodes[i].room.centerX() - roomNodes[centerIdx].room.centerX();
        int roomDirY = roomNodes[i].room.centerY() - roomNodes[centerIdx].room.centerY();
        
        // 내적이 양수면 같은 방향
        if (dirX * roomDirX + dirY * roomDirY > 0) {
            isInDirection = true;
        }
        
        if (isInDirection) {
            int distToCenter = std::abs(roomNodes[i].room.centerX() - roomNodes[centerIdx].room.centerX()) +
                              std::abs(roomNodes[i].room.centerY() - roomNodes[centerIdx].room.centerY());
            
            candidatesCenterToExit.push_back({i, distToCenter});
        }
    }
    
    // 거리에 따라 정렬
    std::sort(candidatesCenterToExit.begin(), candidatesCenterToExit.end(), 
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  return a.second < b.second;
              });
    
    // 중앙과 출구 사이에 1-2개 방 선택
    midPointCount = std::min(2, static_cast<int>(candidatesCenterToExit.size()));
    for (int i = 0; i < midPointCount; i++) {
        mainPathIndices.push_back(candidatesCenterToExit[i].first);
        roomNodes[candidatesCenterToExit[i].first].isOnMainPath = true;
    }
    
    mainPathIndices.push_back(exitIdx);
    
    // 메인 경로 방들 연결
    for (size_t i = 0; i < mainPathIndices.size() - 1; i++) {
        int currentIdx = mainPathIndices[i];
        int nextIdx = mainPathIndices[i + 1];
        
        roomNodes[currentIdx].connections.push_back(nextIdx);
        roomNodes[nextIdx].connections.push_back(currentIdx);
    }
    
    // 루프 완성을 위해 출구에서 입구로 연결 (선택적)
    if (GetRandomBool(0.7f) && mainPathIndices.size() >= 4) {
        // 직접 연결하지 않고 다른 방을 통해 연결
        std::vector<int> remainingRooms;
        for (size_t i = 0; i < roomNodes.size(); i++) {
            if (!roomNodes[i].isOnMainPath) {
                remainingRooms.push_back(i);
            }
        }
        
        if (!remainingRooms.empty()) {
            // 출구와 입구 사이에 위치한 방 찾기
            int bridgeIdx = -1;
            int minDistance = INT_MAX;
            
            for (int idx : remainingRooms) {
                int distToExit = std::abs(roomNodes[idx].room.centerX() - roomNodes[exitIdx].room.centerX()) +
                                std::abs(roomNodes[idx].room.centerY() - roomNodes[exitIdx].room.centerY());
                
                int distToEntry = std::abs(roomNodes[idx].room.centerX() - roomNodes[entranceIdx].room.centerX()) +
                                 std::abs(roomNodes[idx].room.centerY() - roomNodes[entranceIdx].room.centerY());
                
                int totalDist = distToExit + distToEntry;
                
                if (totalDist < minDistance) {
                    minDistance = totalDist;
                    bridgeIdx = idx;
                }
            }
            
            if (bridgeIdx != -1) {
                roomNodes[bridgeIdx].isOnMainPath = true;
                roomNodes[exitIdx].connections.push_back(bridgeIdx);
                roomNodes[bridgeIdx].connections.push_back(exitIdx);
                roomNodes[bridgeIdx].connections.push_back(entranceIdx);
                roomNodes[entranceIdx].connections.push_back(bridgeIdx);
            }
        }
    }
    
    // 나머지 방들을 가지(branch)로 연결
    for (size_t i = 0; i < roomNodes.size(); i++) {
        if (roomNodes[i].isOnMainPath) continue; // 이미 메인 경로에 있는 방은 제외
        
        // 가장 가까운 메인 경로 방 찾기
        int nearestMainPathIdx = -1;
        int minDistance = INT_MAX;
        
        for (size_t j = 0; j < roomNodes.size(); j++) {
            if (!roomNodes[j].isOnMainPath) continue;
            
            int distance = std::abs(roomNodes[i].room.centerX() - roomNodes[j].room.centerX()) +
                          std::abs(roomNodes[i].room.centerY() - roomNodes[j].room.centerY());
            
            if (distance < minDistance) {
                minDistance = distance;
                nearestMainPathIdx = j;
            }
        }
        
        if (nearestMainPathIdx != -1) {
            roomNodes[i].connections.push_back(nearestMainPathIdx);
            roomNodes[nearestMainPathIdx].connections.push_back(i);
        }
    }
    
    // 추가 연결 (순환 경로 생성)
    int extraConnections = GetRandomInt(1, roomNodes.size() / 4);
    
    for (int i = 0; i < extraConnections; i++) {
        int roomIdx1 = GetRandomInt(0, roomNodes.size() - 1);
        int roomIdx2 = GetRandomInt(0, roomNodes.size() - 1);
        
        // 같은 방이거나 이미 연결되어 있으면 스킵
        if (roomIdx1 == roomIdx2) continue;
        
        bool alreadyConnected = false;
        for (int conn : roomNodes[roomIdx1].connections) {
            if (conn == roomIdx2) {
                alreadyConnected = true;
                break;
            }
        }
        
        if (alreadyConnected) continue;
        
        // 추가 연결
        roomNodes[roomIdx1].connections.push_back(roomIdx2);
        roomNodes[roomIdx2].connections.push_back(roomIdx1);
    }
}

// 방 연결 메서드
void DungeonGenerator::ConnectRooms(std::vector<std::vector<int>>& map, const std::vector<RoomNode>& roomNodes) {
    // 각 방의 연결 정보를 기반으로 복도 생성
    for (size_t i = 0; i < roomNodes.size(); i++) {
        for (int connIdx : roomNodes[i].connections) {
            // 중복 연결 방지 (i < connIdx인 경우만 처리)
            if (i < connIdx) {
                // 두 방 사이에 복도 생성
                CreateCorridor(
                    map, 
                    roomNodes[i].room.centerX(), 
                    roomNodes[i].room.centerY(),
                    roomNodes[connIdx].room.centerX(),
                    roomNodes[connIdx].room.centerY()
                );
            }
        }
    }
}

// 복도 생성 메서드
void DungeonGenerator::CreateCorridor(std::vector<std::vector<int>>& map, int x1, int y1, int x2, int y2) {
    // L자형 또는 Z자형 복도 생성
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> coinFlip(0, 2);
    
    int corridorType = coinFlip(gen);
    
    if (corridorType == 0) {
        // L자형 복도 (수평 먼저)
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            if (map[y1][x] == TILE_WALL) {
                map[y1][x] = TILE_FLOOR;
            }
        }
        
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
            if (map[y][x2] == TILE_WALL) {
                map[y][x2] = TILE_FLOOR;
            }
        }
        
        // 복도 교차점에 문 추가 (확률적)
        if (GetRandomBool(0.3f)) {
            map[y1][x2] = TILE_DOOR;
        }
    } else if (corridorType == 1) {
        // L자형 복도 (수직 먼저)
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
            if (map[y][x1] == TILE_WALL) {
                map[y][x1] = TILE_FLOOR;
            }
        }
        
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            if (map[y2][x] == TILE_WALL) {
                map[y2][x] = TILE_FLOOR;
            }
        }
        
        // 복도 교차점에 문 추가 (확률적)
        if (GetRandomBool(0.3f)) {
            map[y2][x1] = TILE_DOOR;
        }
    } else {
        // Z자형 복도 (중간 지점 경유)
        int midX = (x1 + x2) / 2;
        int midY = (y1 + y2) / 2;
        
        // 첫 번째 L자형 (시작점 -> 중간점)
        for (int x = std::min(x1, midX); x <= std::max(x1, midX); x++) {
            if (map[y1][x] == TILE_WALL) {
                map[y1][x] = TILE_FLOOR;
            }
        }
        
        for (int y = std::min(y1, midY); y <= std::max(y1, midY); y++) {
            if (map[y][midX] == TILE_WALL) {
                map[y][midX] = TILE_FLOOR;
            }
        }
        
        // 두 번째 L자형 (중간점 -> 끝점)
        for (int x = std::min(midX, x2); x <= std::max(midX, x2); x++) {
            if (map[midY][x] == TILE_WALL) {
                map[midY][x] = TILE_FLOOR;
            }
        }
        
        for (int y = std::min(midY, y2); y <= std::max(midY, y2); y++) {
            if (map[y][x2] == TILE_WALL) {
                map[y][x2] = TILE_FLOOR;
            }
        }
        
        // 복도 교차점에 문 추가 (확률적)
        if (GetRandomBool(0.3f)) {
            map[y1][midX] = TILE_DOOR;
        }
        if (GetRandomBool(0.3f)) {
            map[midY][x2] = TILE_DOOR;
        }
    }
}

// 특수 방 배치 메서드
void DungeonGenerator::PlaceSpecialRooms(std::vector<std::vector<int>>& map, std::vector<RoomNode>& roomNodes) {
    if (roomNodes.empty()) return;
    
    // 입구 방 찾기
    int entranceIdx = -1;
    for (size_t i = 0; i < roomNodes.size(); i++) {
        if (roomNodes[i].type == ENTRANCE) {
            entranceIdx = i;
            break;
        }
    }
    
    if (entranceIdx == -1) return;
    
    // 출구 방 찾기
    int exitIdx = -1;
    for (size_t i = 0; i < roomNodes.size(); i++) {
        if (roomNodes[i].type == EXIT) {
            exitIdx = i;
            break;
        }
    }
    
    if (exitIdx == -1) return;
    
    // 입구 배치
    int entranceX = roomNodes[entranceIdx].room.centerX();
    int entranceY = roomNodes[entranceIdx].room.centerY();
    map[entranceY][entranceX] = TILE_ENTRANCE;
    
    // 출구 배치
    int exitX = roomNodes[exitIdx].room.centerX();
    int exitY = roomNodes[exitIdx].room.centerY();
    map[exitY][exitX] = TILE_EXIT;
    
    // 문 배치 (방 연결 지점)
    for (size_t i = 0; i < roomNodes.size(); i++) {
        for (int connIdx : roomNodes[i].connections) {
            // 중복 처리 방지
            if (i < connIdx) {
                // 두 방 사이의 중간 지점 찾기
                int x1 = roomNodes[i].room.centerX();
                int y1 = roomNodes[i].room.centerY();
                int x2 = roomNodes[connIdx].room.centerX();
                int y2 = roomNodes[connIdx].room.centerY();
                
                // 방 경계에 문 배치 (간단한 구현)
                // 실제로는 더 복잡한 로직이 필요할 수 있음
                if (std::abs(x1 - x2) > std::abs(y1 - y2)) {
                    // 수평 방향으로 더 멀리 떨어져 있음
                    int doorX = (x1 + x2) / 2;
                    int doorY = y1;
                    
                    // 문 주변이 바닥인지 확인
                    if (map[doorY][doorX] == TILE_FLOOR) {
                        // 30% 확률로 문 배치
                        if (GetRandomBool(0.3f)) {
                            map[doorY][doorX] = TILE_DOOR;
                        }
                    }
                } else {
                    // 수직 방향으로 더 멀리 떨어져 있음
                    int doorX = x1;
                    int doorY = (y1 + y2) / 2;
                    
                    // 문 주변이 바닥인지 확인
                    if (map[doorY][doorX] == TILE_FLOOR) {
                        // 30% 확률로 문 배치
                        if (GetRandomBool(0.3f)) {
                            map[doorY][doorX] = TILE_DOOR;
                        }
                    }
                }
            }
        }
    }
}

// 숨겨진 방 배치 메서드
void DungeonGenerator::PlaceHiddenRooms(std::vector<std::vector<int>>& map, std::vector<RoomNode>& roomNodes) {
    // 숨겨진 방 개수 결정 (1-2개)
    int hiddenRoomCount = GetRandomInt(1, 2);
    
    // 맵 크기 확인
    int height = map.size();
    int width = map[0].size();
    
    for (int i = 0; i < hiddenRoomCount; i++) {
        // 숨겨진 방 크기 결정
        int roomWidth = GetRandomInt(MIN_ROOM_SIZE, MAX_STANDARD_ROOM_SIZE);
        int roomHeight = GetRandomInt(MIN_ROOM_SIZE, MAX_STANDARD_ROOM_SIZE);
        
        // 숨겨진 방 위치 결정 (기존 방과 겹치지 않도록)
        int maxAttempts = 50;
        int attempts = 0;
        bool placed = false;
        
        while (!placed && attempts < maxAttempts) {
            int left = GetRandomInt(1, width - roomWidth - 1);
            int top = GetRandomInt(1, height - roomHeight - 1);
            
            Room hiddenRoom = {
                left,
                top,
                left + roomWidth - 1,
                top + roomHeight - 1
            };
            
            // 기존 방과 겹치는지 확인
            bool overlaps = false;
            for (const auto& node : roomNodes) {
                if (hiddenRoom.intersects(node.room, ROOM_MARGIN)) {
                    overlaps = true;
                    break;
                }
            }
            
            if (!overlaps) {
                // 숨겨진 방 그리기
                DrawRoom(map, hiddenRoom);
                
                // 가장 가까운 일반 방 찾기
                int nearestRoomIdx = -1;
                int minDistance = INT_MAX;
                
                for (size_t j = 0; j < roomNodes.size(); j++) {
                    int distance = std::abs(hiddenRoom.centerX() - roomNodes[j].room.centerX()) +
                                  std::abs(hiddenRoom.centerY() - roomNodes[j].room.centerY());
                    
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearestRoomIdx = j;
                    }
                }
                
                if (nearestRoomIdx != -1) {
                    // 숨겨진 방과 가장 가까운 방 사이에 숨겨진 문 생성
                    int x1 = hiddenRoom.centerX();
                    int y1 = hiddenRoom.centerY();
                    int x2 = roomNodes[nearestRoomIdx].room.centerX();
                    int y2 = roomNodes[nearestRoomIdx].room.centerY();
                    
                    // 복도 생성
                    CreateCorridor(map, x1, y1, x2, y2);
                    
                    // 숨겨진 문 배치
                    if (std::abs(x1 - x2) > std::abs(y1 - y2)) {
                        // 수평 방향으로 더 멀리 떨어져 있음
                        int doorX = (x1 + x2) / 2;
                        int doorY = y1;
                        
                        // 문 주변이 바닥인지 확인
                        if (map[doorY][doorX] == TILE_FLOOR) {
                            map[doorY][doorX] = TILE_HIDDEN_DOOR;
                        }
                    } else {
                        // 수직 방향으로 더 멀리 떨어져 있음
                        int doorX = x1;
                        int doorY = (y1 + y2) / 2;
                        
                        // 문 주변이 바닥인지 확인
                        if (map[doorY][doorX] == TILE_FLOOR) {
                            map[doorY][doorX] = TILE_HIDDEN_DOOR;
                        }
                    }
                }
                
                // 숨겨진 방 노드 추가
                RoomNode hiddenNode(hiddenRoom, HIDDEN);
                roomNodes.push_back(hiddenNode);
                
                placed = true;
            }
            
            attempts++;
        }
    }
}

// 맵 정리 메서드 (불필요한 벽 제거)
void DungeonGenerator::CleanupWalls(std::vector<std::vector<int>>& map) {
    int height = map.size();
    int width = map[0].size();
    
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (map[y][x] == TILE_WALL) {
                // 주변 8방향에 바닥이 하나라도 있는지 확인
                bool hasAdjacentFloor = false;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue;
                        if (map[y + dy][x + dx] == TILE_FLOOR || 
                            map[y + dy][x + dx] == TILE_DOOR || 
                            map[y + dy][x + dx] == TILE_ENTRANCE || 
                            map[y + dy][x + dx] == TILE_EXIT ||
                            map[y + dy][x + dx] == TILE_HIDDEN_DOOR) {
                            hasAdjacentFloor = true;
                            break;
                        }
                    }
                    if (hasAdjacentFloor) break;
                }
                
                // 주변에 바닥이 없으면 불필요한 벽이므로 제거
                if (!hasAdjacentFloor) {
                    map[y][x] = TILE_NONE; // 또는 다른 타일로 변경 가능
                }
            }
        }
    }
}

// 방 그리기 함수
void DungeonGenerator::DrawRoom(std::vector<std::vector<int>>& map, const Room& room, int tileType) {
    for (int y = room.top; y <= room.bottom; y++) {
        for (int x = room.left; x <= room.right; x++) {
            map[y][x] = tileType;
        }
    }
}

// 랜덤 정수 생성 함수
int DungeonGenerator::GetRandomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// 랜덤 불리언 생성 함수
bool DungeonGenerator::GetRandomBool(float probability) {
    return (static_cast<float>(rand()) / RAND_MAX) < probability;
}
