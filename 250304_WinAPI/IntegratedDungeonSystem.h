#pragma once
#include "config.h"
#include "DungeonGenerator.h"
#include "MonsterGenerator.h"
#include "TileVariationManager.h"
#include "EnvironmentalEffectsManager.h"

class Level;

// 통합 던전 시스템 클래스
class IntegratedDungeonSystem {
public:
    IntegratedDungeonSystem();
    ~IntegratedDungeonSystem();

    // 던전 생성 및 초기화
    void GenerateDungeon(Level* level, int width, int height, int floorLevel);

    // 환경 효과 업데이트
    void UpdateEnvironmentalEffects(Level* level, float deltaTime);

    // 환경 효과 적용
    void ApplyEnvironmentalEffect(Level* level, int x, int y, EnvironmentalEffectsManager::EffectType effectType, int duration = 5);

    // 타일 상태 확인
    EnvironmentalEffectsManager::TileState GetTileState(int x, int y) const;

    // 몬스터 생성
    std::vector<Monster*> GenerateMonsters(Level* level, int floorLevel);

private:
    // 던전 생성기
    DungeonGenerator dungeonGenerator;
    
    // 몬스터 생성기
    MonsterGenerator monsterGenerator;
    
    // 타일 변형 관리자
    TileVariationManager tileVariationManager;
    
    // 환경 효과 관리자
    EnvironmentalEffectsManager environmentalEffectsManager;
    
    // 현재 던전 맵 데이터
    std::vector<std::vector<int>> currentMapData;
    
    // 현재 타일 프레임 맵 데이터
    std::vector<std::vector<POINT>> currentFrameMapData;
};
