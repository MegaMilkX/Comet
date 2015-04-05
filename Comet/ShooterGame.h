#pragma once

#include "BaseGame.h"

class ShooterGame : public Comet::BaseGame
{
public:
	ShooterGame();
	~ShooterGame();

	void Init();
	void PostInit();
	void Start();
	bool Update();
private:
	double mx, my;
	double px, py;

	Comet::Entity* camera_;
};

