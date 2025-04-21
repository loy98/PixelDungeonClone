// MonsterGenerator.h
#pragma once
#include "config.h"

class Monster;
class Level;

class MonsterGenerator {
public:
    MonsterGenerator();
    ~MonsterGenerator();

    // 몬스터 생성 및 배치 메서드
    std::vector<Monster*> GenerateMonsters(Level* level, const std::vector<std::vector<int>>& map, int floorLevel);

private:
    // 몬스터 유형 열거형
    enum MonsterType {
        RAT,            // 쥐 (하수도 지역)
        GNOLL,          // 놀 (하수도/감옥 지역)
        CRAB,           // 게 (하수도 지역)
        SKELETON,       // 해골 (감옥 지역)
        THIEF,          // 도둑 (감옥 지역)
        BAT,            // 박쥐 (동굴 지역)
        BRUTE,          // 야만인 (동굴 지역)
        SHAMAN,         // 주술사 (동굴 지역)
        ELEMENTAL,      // 정령 (도시 지역)
        MONK,           // 수도사 (도시 지역)
        WARLOCK,        // 흑마법사 (도시 지역)
        GOLEM,          // 골렘 (도시 지역)
        DEMON           // 악마 (지옥 지역)
    };

    // 지역별 몬스터 유형 정의
    struct RegionMonsters {
        std::vector<MonsterType> commonMonsters;  // 일반 몬스터
        std::vector<MonsterType> rareMonsters;    // 희귀 몬스터
        std::vector<MonsterType> bossMonsters;    // 보스 몬스터
    };

    // 지역별 몬스터 정의
    std::vector<RegionMonsters> regionMonsters;

    // 몬스터 생성 함수
    Monster* CreateMonster(MonsterType type, FPOINT position, float speed);

    // 몬스터 배치 위치 결정 함수
    std::vector<FPOINT> DetermineMonsterPositions(Level* level, const std::vector<std::vector<int>>& map, int count);

    // 지역 및 난이도에 따른 몬스터 수 결정 함수
    int DetermineMonsterCount(int floorLevel, int mapSize);

    // 지역 및 난이도에 따른 몬스터 유형 결정 함수
    std::vector<MonsterType> DetermineMonsterTypes(int floorLevel, int count);

    // 랜덤 유틸리티 함수
    int GetRandomInt(int min, int max);
    bool GetRandomBool(float probability = 0.5f);
    
    // 상수 정의
    const int BASE_MONSTER_COUNT = 3;  // 기본 몬스터 수
    const float MONSTER_COUNT_SCALE = 0.5f;  // 층별 몬스터 수 증가 비율
    const float RARE_MONSTER_CHANCE = 0.2f;  // 희귀 몬스터 등장 확률
    const float BOSS_MONSTER_CHANCE = 0.05f;  // 보스 몬스터 등장 확률 (일반 층)
};
