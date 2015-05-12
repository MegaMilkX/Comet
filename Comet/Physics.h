#pragma once

#include <btBulletDynamicsCommon.h>

#include "RigidBody.h"

namespace Comet
{

	class Physics
	{
	public:
		Physics();
		~Physics();

		void Update();

		RigidBody* CreateRigidBody(btCollisionShape* shape = 0, float mass = 1, bool isKinematic = false);

		btDiscreteDynamicsWorld* GetWorld(){ return world; }
	private:
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;

		btDiscreteDynamicsWorld* world;
	};

}