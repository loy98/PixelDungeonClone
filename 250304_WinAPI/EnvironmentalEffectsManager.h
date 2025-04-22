// EnvironmentalEffectsManager.h
#pragma once
#include "config.h"
#include <vector>
#include <map>
#include <string>

// 전방 선언
class Level;

class EnvironmentalEffectsManager {
public:
    EnvironmentalEffectsManager();
    ~EnvironmentalEffectsManager();

    // 환경 효과 유형 열거형
    enum EffectType {
        EFFECT_NONE = 0,
        EFFECT_FIRE = 1,
        EFFECT_WATER = 2,
        EFFECT_POISON = 3,
        EFFECT_ELECTRICITY = 4,
        EFFECT_ICE = 5
    };

    // 타일 상태 열거형
    enum TileState {
        STATE_NORMAL = 0,
        STATE_BURNING = 1,
        STATE_BURNED = 2,
        STATE_WET = 3,
        STATE_FROZEN = 4,
        STATE_ELECTRIFIED = 5,
        STATE_POISONED = 6
    };

    // 타일 속성 구조체
    struct TileProperties {
        bool isFlammable;
        bool isConductor;
        bool isFreezable;
        bool isDestructible;
        int durability;
        
        TileProperties() : 
            isFlammable(false), 
            isConductor(false), 
            isFreezable(false), 
            isDestructible(false),
            durability(100) {}
    };

    // 환경 효과 초기화
    void Initialize();

    // 타일 속성 설정
    void SetTileProperties(int tileType, const TileProperties& properties);

    // 환경 효과 적용
    void ApplyEffect(Level* level, int x, int y, EffectType effectType, int duration = 5);

    // 환경 효과 업데이트 (매 프레임 호출)
    void Update(Level* level, float deltaTime);

    // 타일 상태 확인
    TileState GetTileState(int x, int y) const;

    // 타일 상태 설정
    void SetTileState(int x, int y, TileState state);

    // 타일 내구도 확인
    int GetTileDurability(int x, int y) const;

    // 타일 내구도 감소
    void DecreaseTileDurability(int x, int y, int amount);

    // 타일 파괴 (내구도가 0이 되었을 때)
    void DestroyTile(Level* level, int x, int y);

    // 효과 전파 (불, 물 등이 주변으로 퍼지는 효과)
    void PropagateEffect(Level* level, int x, int y, EffectType effectType);

private:
    // 타일 상태 맵
    std::map<std::pair<int, int>, TileState> tileStates;
    
    // 타일 내구도 맵
    std::map<std::pair<int, int>, int> tileDurabilities;
    
    // 효과 지속 시간 맵
    std::map<std::pair<int, int>, float> effectDurations;
    
    // 타일 유형별 속성
    std::map<int, TileProperties> tileTypeProperties;
    
    // 효과 업데이트 함수
    void UpdateFireEffect(Level* level, int x, int y, float deltaTime);
    void UpdateWaterEffect(Level* level, int x, int y, float deltaTime);
    void UpdateElectricityEffect(Level* level, int x, int y, float deltaTime);
    void UpdateIceEffect(Level* level, int x, int y, float deltaTime);
    void UpdatePoisonEffect(Level* level, int x, int y, float deltaTime);
    
    // 방향 상수
    const int DIR_COUNT = 4;
    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {-1, 0, 1, 0};
};
