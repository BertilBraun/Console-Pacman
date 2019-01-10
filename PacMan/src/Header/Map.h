#pragma once

#include "ConsoleGameEngine.h"
#include "defines.h"

#include <map>
#include <fstream>

#include "Tile.h"
#include "pos.h"
#include "Points.h"
#include "PacMan.h"
#include "Ghost.h"

class Map {
public:
	Map(const std::wstring& path);
	~Map();

	bool UpdatePoints(PacMan& pacman, Ghost* g[4]);
	bool LoadMap(const std::wstring& path);

	Tile* TileMap = nullptr;
	Sprite* mapSprite = nullptr;
	std::map<Posf, Point> points; //USED TO UPDATE
	std::map<Posf, Point> allPoints; //USED TO RESET
	bool corrupted = false;
};

