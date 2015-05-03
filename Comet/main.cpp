#pragma strict_gs_check( on ) 

#include "AdventureGame.h"
#include "ShooterGame.h"

#include <math3f.h>

int main()
{
	ShooterGame * game = new ShooterGame();
	game->Init();

	while (game->Update())
	{
	}

	delete game;
	return 0;
}