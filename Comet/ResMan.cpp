#include "ResMan.h"

namespace Comet
{
	ResMan* ResMan::instance;

	ResMan::ResMan()
	{
		instance = 0;
	}


	ResMan::~ResMan()
	{
		std::map<std::string, Resource*>::iterator it;
		for (it = resources.begin(); it != resources.end(); it++)
		{
			delete it->second;
		}
	}

	void ResMan::Initialize()
	{
		instance = new ResMan();

		printf("[OK]Resource Manager init OK.\n");
	}

	void ResMan::Destroy()
	{
		if (instance)
			delete instance;
	}

};