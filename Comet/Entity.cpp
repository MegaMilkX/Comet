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

	void Entity::SetTransform(Node* n){ transform = AddComponent(n); }
	void Entity::SetRenderable(Renderable* r){ renderable = AddComponent(r); }
	void Entity::SetRigidBody(RigidBody* rb){ rigidBody = AddComponent(rb); }
}