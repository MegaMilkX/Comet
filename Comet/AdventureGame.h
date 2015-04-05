#pragma once
#include "BaseGame.h"

#include "WorldMapScene.h"

#include <vector>

#include "Party.h"

class AdventureGame : public Comet::BaseGame
{
public:
	AdventureGame();
	~AdventureGame();

	void Init();
	void PostInit();
	void Start();
	bool Update();
private:
	GameScene* currentScene;
	WorldMapScene* worldMap;
	int camera;

	//test
	Comet::TextRenderable* text;
	Party* party;
};

