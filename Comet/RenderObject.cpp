#include "Node.h"
#include "RenderObject.h"

#include <sstream>

#include "Entity.h"

namespace Comet
{

	RenderObject::RenderObject()
	{
		node = 0;
		std::stringstream ss;
		ss.clear();
		ss << "object_" << this;

		name = ss.str();
	}


	RenderObject::~RenderObject()
	{
	}

	void RenderObject::SetEntity(Entity* e)
	{
		Component::SetEntity(e);
		//TODO Get an entity's transform node and attach to it
		e->GetComponent<Node>()->Attach(this);
	}
};