#pragma strict_gs_check( on ) 

#include "AdventureGame.h"
#include "ShooterGame.h"

#include <math3f.h>

int main()
{
	ShooterGame * game = new ShooterGame();
	game->Init();

	vec3f a(1, 4, 0);
	vec3f b(5, 1, 5);
	vec3f r = a + b;
	r = 3 * r;
	r = r * 3;
	r = r / 3;
	r *= 6;
	r += a;
	r = r / 2;
	r /= 2;
	r = -r;
	//normalize(r);
	r.length();

	while (game->Update())
	{
	}

	delete game;
	return 0;
}