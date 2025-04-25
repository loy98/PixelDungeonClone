// IntegratedDungeonSystem.cpp
#include "IntegratedDungeonSystem.h"
#include "Level.h"
#include <random>
#include <ctime>

#include "FileLoadDungeonGenerator.h"
#include "ProceduralDungeonGenerator.h"

void IntegratedDungeonSystem::GenerateDungeon(bool isProcedural, Level* level, int width, int height, int roomCount, int minRoomSize,
    int maxRoomSize, WCHAR* filepath)
{
    if (isProcedural == true) {
        dungeonGenerator = new ProceduralDungeonGenerator();
    }
    else {
        wstring wFileName = GetFileNameFromWCHAR(filepath);
        string fileName = WStringToString(wFileName);
        dungeonGenerator = new FileLoadDungeonGenerator(fileName);
    }
   
    
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

wstring IntegratedDungeonSystem::GetFileNameFromWCHAR(const WCHAR* fullPath)
{
    std::wstring path(fullPath);
    size_t pos = path.find_last_of(L"\\/");
    if (pos == std::wstring::npos) return path;
    return path.substr(pos + 1);
}

string IntegratedDungeonSystem::WStringToString(const std::wstring& wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], -1, &result[0], size_needed, NULL, NULL);
    return result;
}
