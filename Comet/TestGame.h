#pragma once

#include "Game.h"

class TestGame : public Comet::Game
{
public:
	TestGame();
	~TestGame();

	void Init();
	void Update();

private:
	/*TEMP*/
	double mx, my;
	double px, py;
	Comet::Viewport* viewport;
};

