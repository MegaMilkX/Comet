#include "Component.h"

namespace Comet
{

	Component::Component()
	{
		entityBad = false;
	}


	Component::~Component()
	{
	}

	void Component::SetEntity(Entity* e)
	{
		entity = e;
	}

	void Component::BadEntity()
	{ entityBad = true; }
}