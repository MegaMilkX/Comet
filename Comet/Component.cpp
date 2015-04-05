#include "Component.h"

namespace Comet
{

	Component::Component()
	{
	}


	Component::~Component()
	{
	}

	void Component::SetEntity(Entity* e)
	{
		entity = e;
	}
}