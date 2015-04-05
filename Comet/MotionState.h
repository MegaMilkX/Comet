#pragma once

#include <btBulletDynamicsCommon.h>

namespace Comet
{
	class Node;
	class RigidBody;

	class MotionState : public btMotionState
	{
	public:
		MotionState(Node* n = 0);
		~MotionState();

		void getWorldTransform(btTransform& trans) const;
		void setWorldTransform(const btTransform& trans);

		void SetNode(Node* n){ node = n; }
	private:
		Node* node;
	};

}