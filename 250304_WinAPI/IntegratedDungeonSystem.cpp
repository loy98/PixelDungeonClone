// IntegratedDungeonSystem.cpp
#include "IntegratedDungeonSystem.h"
#include "Level.h"
#include <random>
#include <ctime>

IntegratedDungeonSystem::IntegratedDungeonSystem() {
    // 랜덤 시드 초기화
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

IntegratedDungeonSystem::~IntegratedDungeonSystem() {
}

// 던전 생성 및 초기화
void IntegratedDungeonSystem::GenerateDungeon(Level* level, int width, int height, int floorLevel) {
    if (!level) return;
    
    // 1. 던전 맵 생성
    currentMapData = dungeonGenerator.Generate(width, height);
    
    // 2. 타일 변형 결정
    // currentFrameMapData = tileVariationManager.ProcessTileMap(currentMapData);
    
    // 3. 레벨에 맵 데이터 설정
    level->SetMapData(currentMapData);
    // level->SetFrameMapData(currentFrameMapData);
    
    // 4. 몬스터 생성
    std::vector<Monster*> monsters = GenerateMonsters(level, floorLevel);
    
    // 5. 레벨에 몬스터 추가
    level->AddMonsters(monsters);
    
    // 6. 환경 효과 초기화
    environmentalEffectsManager.Initialize();
}

// 환경 효과 업데이트
void IntegratedDungeonSystem::UpdateEnvironmentalEffects(Level* level, float deltaTime) {
    environmentalEffectsManager.Update(level, deltaTime);
}

// 환경 효과 적용
void IntegratedDungeonSystem::ApplyEnvironmentalEffect(Level* level, int x, int y, EnvironmentalEffectsManager::EffectType effectType, int duration) {
    environmentalEffectsManager.ApplyEffect(level, x, y, effectType, duration);
}

// 타일 상태 확인
EnvironmentalEffectsManager::TileState IntegratedDungeonSystem::GetTileState(int x, int y) const {
    return environmentalEffectsManager.GetTileState(x, y);
}

// 몬스터 생성
std::vector<Monster*> IntegratedDungeonSystem::GenerateMonsters(Level* level, int floorLevel) {
    return monsterGenerator.GenerateMonsters(level, currentMapData, floorLevel);
}
