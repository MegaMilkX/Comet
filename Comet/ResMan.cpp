#include "ResMan.h"

#include "Material.h"
#include "Shader.h"

namespace Comet
{
	
	ResMan &Resources()
	{
		static ResMan resManager;
		return resManager;
	}

	ResMan::ResMan()
	{
		Initialize();
	}


	ResMan::~ResMan()
	{
		std::map<std::string, Resource*>::iterator it;
		for (it = resources.begin(); it != resources.end(); it++)
		{
			delete (it->second);
		}
	}

	void ResMan::Initialize()
	{

		

		printf("[OK]Resource Manager init OK.\n");
	}

	void ResMan::Destroy()
	{
	}

};