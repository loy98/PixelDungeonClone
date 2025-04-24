// IntegratedDungeonSystem.cpp
#include "IntegratedDungeonSystem.h"
#include "Level.h"
#include <random>
#include <ctime>

#include "FileLoadDungeonGenerator.h"
#include "ProceduralDungeonGenerator.h"
//
// IntegratedDungeonSystem::IntegratedDungeonSystem() {
//     // Initialize random seed
//     std::srand(static_cast<unsigned int>(std::time(nullptr)));
// }
//
// IntegratedDungeonSystem::~IntegratedDungeonSystem() {
// }
//
// // Generate dungeon and initialize
// void IntegratedDungeonSystem::GenerateDungeon(Level* level, int width, int height, int floorLevel) {
//     if (!level) return;
//     
//     // 1. Generate dungeon map
//     currentMapData = dungeonGenerator.Generate(width, height);
//     
//     // 2. Determine tile variations
//     currentFrameMapData = tileVariationManager.ProcessTileMap(currentMapData);
//     
//     // 3. Set map data in level
//     level->SetMapData(currentMapData);
//     level->SetFrameMapData(currentFrameMapData);
//     
//     // 4. Generate monsters
//     std::vector<Monster*> monsters = GenerateMonsters(level, floorLevel);
//     
//     // 5. Add monsters to level
//     level->AddMonsters(monsters);
//     
//     // 6. Initialize environmental effects - REMOVED
//     // environmentalEffectsManager.Initialize();
// }
//
// // Generate monsters
// std::vector<Monster*> IntegratedDungeonSystem::GenerateMonsters(Level* level, int floorLevel) {
//     return monsterGenerator.GenerateMonsters(level, currentMapData, floorLevel);
// }
void IntegratedDungeonSystem::GenerateDungeon(Level* level, int width, int height, int roomCount, int minRoomSize,
    int maxRoomSize)
{
    dungeonGenerator = new FileLoadDungeonGenerator("TileMapData.dat");
    // dungeonGenerator = new ProceduralDungeonGenerator();
    // dungeonGenerator = new DungeonGenerator();
    
    // Generate the dungeon layout
    auto map = dungeonGenerator->Generate(width, height);
    auto renderMap = dungeonGenerator->GetTileVariations(map);
    
    // Get the generated map data
    // std::vector<std::vector<int>> mapData = dungeonGenerator.GetMapData();
    //
    // Set the map data in the level
    level->SetMapData(map);
    level->SetRenderMap(renderMap);
    
    // Generate monsters based on the dungeon layout
    std::vector<Monster*> monsters = monsterGenerator.GenerateMonsters(level, map, 2); // Generate 10 monsters
        
    // Add the monsters to the level
    level->AddMonsters(monsters);
}
