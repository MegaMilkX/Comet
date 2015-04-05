#include "Resource.h"

namespace Comet
{

	Resource::Resource()
	{
		isReady = false;
	}


	Resource::~Resource()
	{
		Unload();
	}

	void Resource::Unload()
	{

	}

};