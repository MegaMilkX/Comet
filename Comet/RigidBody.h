#pragma once

#include <btBulletDynamicsCommon.h>
#include "Component.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Comet
{
	class Physics;
	class MotionState;
	class RigidBody : public Component
	{
	public:
		RigidBody(Physics* physics, btCollisionShape* shape, float mass = 1, bool isKinematic = false);
		~RigidBody();

		void SetPosition(glm::vec3 pos);
		void SetMass(float val);
		void SetKinematic(bool val);


		//
		void SetEntity(Entity* e);

		btRigidBody* GetBody(){ return rigidBody; }
	private:
		btRigidBody* rigidBody;
		btCollisionShape* collisionShape;
		MotionState* motionState;
		Physics* physics;
	};

}