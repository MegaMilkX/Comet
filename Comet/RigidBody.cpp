#include "RigidBody.h"

#include "Physics.h"

#include "MotionState.h"

#include "Entity.h"

namespace Comet
{

	RigidBody::RigidBody(Physics* physics, btCollisionShape* shape, float mass, bool isKinematic)
	{
		if (!shape || !physics)
			return;

		collisionShape = shape;
		btVector3 localInertia;
		collisionShape->calculateLocalInertia(mass, localInertia);
		motionState = new MotionState();
		btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, motionState, collisionShape, localInertia);
		rigidBody = new btRigidBody(sphereRigidBodyCI);
		rigidBody->setFriction(0.5f);
		rigidBody->setRollingFriction(0.5f);

		if (isKinematic)
		{
			rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			rigidBody->setActivationState(DISABLE_DEACTIVATION);
		}
		
		this->physics = physics;
		physics->GetWorld()->addRigidBody(rigidBody);
		
	}


	RigidBody::~RigidBody()
	{
		physics->GetWorld()->removeRigidBody(rigidBody);
		delete rigidBody->getMotionState();
		delete rigidBody;
	}

	void RigidBody::SetPosition(glm::vec3 pos)
	{
		rigidBody->setCenterOfMassTransform(btTransform(btQuaternion(), btVector3(pos.x, pos.y, pos.z)));
	}

	void RigidBody::SetMass(float val)
	{

	}

	void RigidBody::SetKinematic(bool val)
	{

	}


	//-----------------------------------------
	void RigidBody::SetEntity(Entity* e)
	{
		Component::SetEntity(e);
		motionState->SetNode(entity->GetComponent<Node>());
	}
}