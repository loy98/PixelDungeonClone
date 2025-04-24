#pragma once
#include "IDungeonGenerator.h"
#include <string>
#include <fstream>

class FileLoadDungeonGenerator : public IDungeonGenerator {
public:
    FileLoadDungeonGenerator(const std::string& filePath);
    virtual ~FileLoadDungeonGenerator() = default;

    // IDungeonGenerator 인터페이스 구현
    virtual std::vector<std::vector<int>> Generate(int width, int height) override;
    virtual std::vector<std::vector<int>> GetTileVariations(std::vector<std::vector<int>>& map) override;
    virtual void RenderWallTile(D2DImage* image, int x, int y, int tileX, int tileY, float scale,
                        const std::vector<std::vector<int>>& map) override;
private:
    std::string filePath;
    std::vector<std::vector<int>> dungeonData;
    
    // 파일에서 던전 데이터를 로드하는 메서드
    bool LoadDungeonData();
    
    // 던전 데이터를 검증하는 메서드
    bool ValidateDungeonData(int expectedWidth, int expectedHeight);
    
    // 타일 변형 결정 메서드들
    int DetermineWallVariation(const std::vector<std::vector<int>>& map, int x, int y);
    int DetermineFloorVariation(const std::vector<std::vector<int>>& map, int x, int y);

    // 던전 데이터 로드 및 검증
    // bool LoadDungeonData();
    // bool ValidateDungeonData(int width, int height);
    
    // 특수 타일 배치
    void PlaceSpecialRooms(std::vector<std::vector<int>>& map);
    void PlaceDoorsAtRoomBorders(std::vector<std::vector<int>>& map);
    
    // 유틸리티 함수
    bool IsValidDoorLocation(const std::vector<std::vector<int>>& map, int x, int y) const;
    bool GetRandomBool(float probability) const;
}; 