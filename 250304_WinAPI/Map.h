#pragma once
#include "config.h"

#define TILE_WIDTH      16
#define TILE_HEIGHT     16
#define MAP_WIDTH       40
#define MAP_HEIGHT      40

class Map
{
    int width;
    int height;
    int** tiles;
public:
    Map(int width = TILE_WIDTH, int height = TILE_HEIGHT);

    bool Load(wchar_t* filePath);
    bool CanGo(FPOINT nextPos);

    POINT GetRandomFloorTile();
    int GetTileType(int x, int y);
    inline int GetWidth(){ return width; }
    inline int GetHeight(){ return height; }
};
