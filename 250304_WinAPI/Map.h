#pragma once
typedef enum class TileType {
	NONE,
	WALL,
	FLOOR,
	COUNT
}TT;


struct Map {
	int frameX;
	int frameY;
	TT type;
};
