// EnvironmentalEffectsManager.cpp
#include "EnvironmentalEffectsManager.h"
#include "Level.h"
#include <random>
#include <ctime>

EnvironmentalEffectsManager::EnvironmentalEffectsManager() {
    // 랜덤 시드 초기화
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // 환경 효과 초기화
    Initialize();
}

EnvironmentalEffectsManager::~EnvironmentalEffectsManager() {
}

// 환경 효과 초기화
void EnvironmentalEffectsManager::Initialize() {
    // 기본 타일 속성 설정
    
    // 벽 타일 속성
    TileProperties wallProps;
    wallProps.isFlammable = false;
    wallProps.isConductor = false;
    wallProps.isFreezable = false;
    wallProps.isDestructible = false;
    wallProps.durability = 1000;
    SetTileProperties(1, wallProps); // TILE_WALL = 1
    
    // 바닥 타일 속성
    TileProperties floorProps;
    floorProps.isFlammable = false;
    floorProps.isConductor = false;
    floorProps.isFreezable = false;
    floorProps.isDestructible = false;
    floorProps.durability = 1000;
    SetTileProperties(2, floorProps); // TILE_FLOOR = 2
    
    // 문 타일 속성 (불에 탐)
    TileProperties doorProps;
    doorProps.isFlammable = true;
    doorProps.isConductor = false;
    doorProps.isFreezable = false;
    doorProps.isDestructible = true;
    doorProps.durability = 100;
    SetTileProperties(3, doorProps); // TILE_DOOR = 3
    
    // 숨겨진 문 타일 속성 (불에 탐)
    TileProperties hiddenDoorProps;
    hiddenDoorProps.isFlammable = true;
    hiddenDoorProps.isConductor = false;
    hiddenDoorProps.isFreezable = false;
    hiddenDoorProps.isDestructible = true;
    hiddenDoorProps.durability = 100;
    SetTileProperties(6, hiddenDoorProps); // TILE_HIDDEN_DOOR = 6
}

// 타일 속성 설정
void EnvironmentalEffectsManager::SetTileProperties(int tileType, const TileProperties& properties) {
    tileTypeProperties[tileType] = properties;
}

// 환경 효과 적용
void EnvironmentalEffectsManager::ApplyEffect(Level* level, int x, int y, EffectType effectType, int duration) {
    if (!level) return;
    
    // 맵 범위 확인
    std::vector<std::vector<int>> map = level->GetMapData();
    if (y < 0 || y >= map.size() || x < 0 || x >= map[0].size()) return;
    
    // 타일 유형 확인
    int tileType = map[y][x];
    
    // 타일 속성 확인
    if (tileTypeProperties.find(tileType) == tileTypeProperties.end()) return;
    
    // 효과 유형에 따른 처리
    switch (effectType) {
        case EFFECT_FIRE:
            // 불에 탈 수 있는 타일인 경우
            if (tileTypeProperties[tileType].isFlammable) {
                // 타일 상태를 불타는 상태로 변경
                SetTileState(x, y, STATE_BURNING);
                // 효과 지속 시간 설정
                effectDurations[{x, y}] = static_cast<float>(duration);
                // 효과 전파
                PropagateEffect(level, x, y, EFFECT_FIRE);
            }
            break;
            
        case EFFECT_WATER:
            // 물에 젖을 수 있는 타일인 경우
            SetTileState(x, y, STATE_WET);
            effectDurations[{x, y}] = static_cast<float>(duration);
            // 불 효과 제거
            if (GetTileState(x, y) == STATE_BURNING) {
                SetTileState(x, y, STATE_WET);
            }
            // 효과 전파
            PropagateEffect(level, x, y, EFFECT_WATER);
            break;
            
        case EFFECT_ELECTRICITY:
            // 전기가 통하는 타일인 경우
            if (tileTypeProperties[tileType].isConductor || GetTileState(x, y) == STATE_WET) {
                SetTileState(x, y, STATE_ELECTRIFIED);
                effectDurations[{x, y}] = static_cast<float>(duration);
                // 효과 전파
                PropagateEffect(level, x, y, EFFECT_ELECTRICITY);
            }
            break;
            
        case EFFECT_ICE:
            // 얼 수 있는 타일인 경우
            if (tileTypeProperties[tileType].isFreezable || GetTileState(x, y) == STATE_WET) {
                SetTileState(x, y, STATE_FROZEN);
                effectDurations[{x, y}] = static_cast<float>(duration);
            }
            break;
            
        case EFFECT_POISON:
            // 독 효과
            SetTileState(x, y, STATE_POISONED);
            effectDurations[{x, y}] = static_cast<float>(duration);
            // 효과 전파
            PropagateEffect(level, x, y, EFFECT_POISON);
            break;
            
        default:
            break;
    }
}

// 환경 효과 업데이트 (매 프레임 호출)
void EnvironmentalEffectsManager::Update(Level* level, float deltaTime) {
    if (!level) return;
    
    // 모든 효과 타일 업데이트
    std::vector<std::pair<int, int>> tilesToUpdate;
    
    for (auto& entry : tileStates) {
        int x = entry.first.first;
        int y = entry.first.second;
        TileState state = entry.second;
        
        tilesToUpdate.push_back({x, y});
    }
    
    // 타일 업데이트
    for (const auto& tile : tilesToUpdate) {
        int x = tile.first;
        int y = tile.second;
        TileState state = GetTileState(x, y);
        
        switch (state) {
            case STATE_BURNING:
                UpdateFireEffect(level, x, y, deltaTime);
                break;
                
            case STATE_WET:
                UpdateWaterEffect(level, x, y, deltaTime);
                break;
                
            case STATE_ELECTRIFIED:
                UpdateElectricityEffect(level, x, y, deltaTime);
                break;
                
            case STATE_FROZEN:
                UpdateIceEffect(level, x, y, deltaTime);
                break;
                
            case STATE_POISONED:
                UpdatePoisonEffect(level, x, y, deltaTime);
                break;
                
            default:
                break;
        }
    }
}

// 불 효과 업데이트
void EnvironmentalEffectsManager::UpdateFireEffect(Level* level, int x, int y, float deltaTime) {
    // 지속 시간 감소
    if (effectDurations.find({x, y}) != effectDurations.end()) {
        effectDurations[{x, y}] -= deltaTime;
        
        // 내구도 감소
        if (GetTileDurability(x, y) > 0) {
            DecreaseTileDurability(x, y, 1);
        }
        
        // 지속 시간이 끝나면
        if (effectDurations[{x, y}] <= 0) {
            // 내구도가 0이면 타일 파괴
            if (GetTileDurability(x, y) <= 0) {
                DestroyTile(level, x, y);
            } else {
                // 불탄 상태로 변경
                SetTileState(x, y, STATE_BURNED);
            }
            
            // 효과 지속 시간 제거
            effectDurations.erase({x, y});
        }
        
        // 일정 확률로 주변으로 불이 번짐
        if (rand() % 100 < 10) { // 10% 확률
            PropagateEffect(level, x, y, EFFECT_FIRE);
        }
    }
}

// 물 효과 업데이트
void EnvironmentalEffectsManager::UpdateWaterEffect(Level* level, int x, int y, float deltaTime) {
    // 지속 시간 감소
    if (effectDurations.find({x, y}) != effectDurations.end()) {
        effectDurations[{x, y}] -= deltaTime;
        
        // 지속 시간이 끝나면
        if (effectDurations[{x, y}] <= 0) {
            // 일반 상태로 변경
            SetTileState(x, y, STATE_NORMAL);
            
            // 효과 지속 시간 제거
            effectDurations.erase({x, y});
        }
    }
}

// 전기 효과 업데이트
void EnvironmentalEffectsManager::UpdateElectricityEffect(Level* level, int x, int y, float deltaTime) {
    // 지속 시간 감소
    if (effectDurations.find({x, y}) != effectDurations.end()) {
        effectDurations[{x, y}] -= deltaTime;
        
        // 지속 시간이 끝나면
        if (effectDurations[{x, y}] <= 0) {
            // 일반 상태로 변경
            SetTileState(x, y, STATE_NORMAL);
            
            // 효과 지속 시간 제거
            effectDurations.erase({x, y});
        }
    }
}

// 얼음 효과 업데이트
void EnvironmentalEffectsManager::UpdateIceEffect(Level* level, int x, int y, float deltaTime) {
    // 지속 시간 감소
    if (effectDurations.find({x, y}) != effectDurations.end()) {
        effectDurations[{x, y}] -= deltaTime;
        
        // 지속 시간이 끝나면
        if (effectDurations[{x, y}] <= 0) {
            // 젖은 상태로 변경 (얼음이 녹음)
            SetTileState(x, y, STATE_WET);
            
            // 새로운 지속 시간 설정 (젖은 상태)
            effectDurations[{x, y}] = 3.0f;
        }
    }
}

// 독 효과 업데이트
void EnvironmentalEffectsManager::UpdatePoisonEffect(Level* level, int x, int y, float deltaTime) {
    // 지속 시간 감소
    if (effectDurations.find({x, y}) != effectDurations.end()) {
        effectDurations[{x, y}] -= deltaTime;
        
        // 지속 시간이 끝나면
        if (effectDurations[{x, y}] <= 0) {
            // 일반 상태로 변경
            SetTileState(x, y, STATE_NORMAL);
            
            // 효과 지속 시간 제거
            effectDurations.erase({x, y});
        }
    }
}

// 타일 상태 확인
EnvironmentalEffectsManager::TileState EnvironmentalEffectsManager::GetTileState(int x, int y) const {
    auto it = tileStates.find({x, y});
    if (it != tileStates.end()) {
        return it->second;
    }
    return STATE_NORMAL;
}

// 타일 상태 설정
void EnvironmentalEffectsManager::SetTileState(int x, int y, TileState state) {
    tileStates[{x, y}] = state;
}

// 타일 내구도 확인
int EnvironmentalEffectsManager::GetTileDurability(int x, int y) const {
    auto it = tileDurabilities.find({x, y});
    if (it != tileDurabilities.end()) {
        return it->second;
    }
    
    // 기본 내구도 반환
    return 100;
}

// 타일 내구도 감소
void EnvironmentalEffectsManager::DecreaseTileDurability(int x, int y, int amount) {
    // 내구도가 없으면 초기화
    if (tileDurabilities.find({x, y}) == tileDurabilities.end()) {
        // 맵에서 타일 유형 확인
        // 여기서는 예시로 기본값 사용
        tileDurabilities[{x, y}] = 100;
    }
    
    // 내구도 감소
    tileDurabilities[{x, y}] = (std::max)(0, tileDurabilities[{x, y}] - amount);
}

// 타일 파괴 (내구도가 0이 되었을 때)
void EnvironmentalEffectsManager::DestroyTile(Level* level, int x, int y) {
    if (!level) return;
    
    // 맵 데이터 가져오기
    std::vector<std::vector<int>> map = level->GetMapData();
    if (y < 0 || y >= map.size() || x < 0 || x >= map[0].size()) return;
    
    // 타일 유형 확인
    int tileType = map[y][x];
    
    // 문인 경우 바닥으로 변경
    if (tileType == 3 || tileType == 6) { // TILE_DOOR 또는 TILE_HIDDEN_DOOR
        level->SetTile(x, y, 2); // TILE_FLOOR = 2
    }
    
    // 타일 상태 및 내구도 초기화
    tileStates.erase({x, y});
    tileDurabilities.erase({x, y});
    effectDurations.erase({x, y});
}

// 효과 전파 (불, 물 등이 주변으로 퍼지는 효과)
void EnvironmentalEffectsManager::PropagateEffect(Level* level, int x, int y, EffectType effectType) {
    if (!level) return;
    
    // 맵 데이터 가져오기
    std::vector<std::vector<int>> map = level->GetMapData();
    if (y < 0 || y >= map.size() || x < 0 || x >= map[0].size()) return;
    
    // 효과 유형에 따른 전파 확률
    int propagationChance = 0;
    switch (effectType) {
        case EFFECT_FIRE:
            propagationChance = 30; // 30% 확률로 불이 번짐
            break;
        case EFFECT_WATER:
            propagationChance = 50; // 50% 확률로 물이 번짐
            break;
        case EFFECT_ELECTRICITY:
            propagationChance = 70; // 70% 확률로 전기가 번짐 (물이 있는 경우)
            break;
        case EFFECT_POISON:
            propagationChance = 20; // 20% 확률로 독이 번짐
            break;
        default:
            return;
    }
    
    // 주변 4방향 확인
    for (int dir = 0; dir < DIR_COUNT; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        
        // 맵 범위 확인
        if (ny < 0 || ny >= map.size() || nx < 0 || nx >= map[0].size()) continue;
        
        // 타일 유형 확인
        int nextTileType = map[ny][nx];
        
        // 이미 같은 효과가 적용된 타일은 스킵
        if (GetTileState(nx, ny) == STATE_BURNING && effectType == EFFECT_FIRE) continue;
        if (GetTileState(nx, ny) == STATE_WET && effectType == EFFECT_WATER) continue;
        if (GetTileState(nx, ny) == STATE_ELECTRIFIED && effectType == EFFECT_ELECTRICITY) continue;
        if (GetTileState(nx, ny) == STATE_POISONED && effectType == EFFECT_POISON) continue;
        
        // 확률에 따라 효과 전파
        if (rand() % 100 < propagationChance) {
            // 효과 적용 (지속 시간은 원래보다 짧게)
            ApplyEffect(level, nx, ny, effectType, 3);
        }
    }
}
