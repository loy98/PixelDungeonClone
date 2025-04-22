#pragma once
#include "config.h"
#include "DungeonGenerator.h"
#include "MonsterGenerator.h"

class Level;
class IntegratedDungeonSystem {
private:
    DungeonGenerator dungeonGenerator;
    MonsterGenerator monsterGenerator;

public:
    IntegratedDungeonSystem() = default;
    ~IntegratedDungeonSystem() = default;

    // Generate a complete dungeon with monsters
    void GenerateDungeon(Level* level, int width, int height, int roomCount, int minRoomSize, int maxRoomSize);
};
