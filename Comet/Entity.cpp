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

	void Entity::SetTransform(Node* n){ _setTransform(AddComponent(n)); }
	void Entity::SetRenderable(Renderable* r){ _setRenderable(AddComponent(r)); }
	void Entity::SetRigidBody(RigidBody* rb){ _setRigidBody(AddComponent(rb)); }
}