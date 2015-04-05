#include "Entity.h"

#include "Node.h"
#include "Renderable.h"
#include "RigidBody.h"

namespace Comet
{

	Entity::Entity()
	{
	}


	Entity::~Entity()
	{
	}

	Node* Entity::GetTransform()
	{
		return 0;
	}

	Renderable* Entity::GetRenderable()
	{
		return 0;
	}

	RigidBody* Entity::GetRigidBody()
	{
		return 0;
	}

}