#include "WorldTileMap.h"

#include "AdventureGame.h"

WorldTileMap::WorldTileMap(AdventureGame* game)
{
	this->game = game;
}


WorldTileMap::~WorldTileMap()
{
}

void WorldTileMap::Init(int w, int h)
{
	tiles.resize(w);
	for (int i = 0; i < w; i++)
		tiles[i].resize(h);

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			tiles[i][j] = new WorldMapTile(game, i, j);
		}
	}
}