#include "Core.h"
#include "TestGame.h"

int main()
{
	Comet::Core* comet = new Comet::Core();
	comet->Init();
	comet->SetGame<TestGame>();
	comet->Reset();

	delete comet;
	return 0;
}