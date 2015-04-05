#pragma once

#include "GameScene.h"

#include "WorldTileMap.h"

#include "Core.h"

class WorldMapScene : public GameScene
{
public:
	WorldMapScene(AdventureGame* game);
	~WorldMapScene();

	void Init();
	void OnSwitch(bool on);
	void Update();
private:
	AdventureGame*		game;

	WorldTileMap*		tileMap;

	Comet::Viewport*	viewport;
	Comet::Camera*		camera;
	Comet::Node*		nodeCamera;
};

