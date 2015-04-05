#pragma once

#include <vector>
#include "WorldMapTile.h"

class AdventureGame;

class WorldTileMap
{
public:
	WorldTileMap(AdventureGame* game);
	~WorldTileMap();

	void Init(int w, int h);
private:
	AdventureGame* game;
	std::vector<std::vector<WorldMapTile*>> tiles;
};

