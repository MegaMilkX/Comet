#pragma once

#include "BaseGame.h"

class Player
{
public:
	Player(Comet::Core* core);
	~Player();

	void Update();
private:
	Comet::Entity* entity;
};

