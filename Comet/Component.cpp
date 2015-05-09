#include "Component.h"

namespace Comet
{

	Component::Component()
	{
	}


	Component::~Component()
	{
	}

	void Component::SetNode(Node* e)
	{
		node = e;
	}
}