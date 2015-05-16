#include "Resource.h"

namespace Comet
{

	Resource::Resource(bool make_default)
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