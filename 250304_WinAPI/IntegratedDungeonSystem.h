#pragma once
#include "config.h"
#include "DungeonGenerator.h"
#include "MonsterGenerator.h"

class IDungeonGenerator;
class Level;
class IntegratedDungeonSystem {
private:
    IDungeonGenerator* dungeonGenerator;
    MonsterGenerator monsterGenerator;

public:
    IntegratedDungeonSystem() = default;
    ~IntegratedDungeonSystem() = default;

    // Generate a complete dungeon with monsters
    void GenerateDungeon(Level* level, int width, int height, int roomCount, int minRoomSize, int maxRoomSize);
};
