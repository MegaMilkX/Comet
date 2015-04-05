#pragma once

class AdventureGame;

class GameScene
{
public:
	GameScene();
	~GameScene();

	virtual void Init() = 0;
	virtual void OnSwitch(bool on) = 0;
	virtual void Update() = 0;
protected:
	AdventureGame* game;
};

