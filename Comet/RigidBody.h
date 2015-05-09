#pragma once

#include <btBulletDynamicsCommon.h>
#include "Component.h"

#include <math3f.h>

namespace Comet
{
	class Physics;
	class MotionState;
	class RigidBody : public Component
	{
	public:
		RigidBody(Physics* physics, btCollisionShape* shape, float mass = 1, bool isKinematic = false);
		~RigidBody();

		void SetPosition(vec3f pos);
		void SetMass(float val);
		void SetKinematic(bool val);

		btRigidBody* GetBody(){ return rigidBody; }
	private:
		btRigidBody* rigidBody;
		btCollisionShape* collisionShape;
		MotionState* motionState;
		Physics* physics;
	};

}