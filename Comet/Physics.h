#pragma once

#include <btBulletDynamicsCommon.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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