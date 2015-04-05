#include "MotionState.h"
#include "Node.h"

namespace Comet
{

	MotionState::MotionState(Node* n)
	{
		node = n;
	}


	MotionState::~MotionState()
	{
	}

	void MotionState::getWorldTransform(btTransform& trans) const
	{
		if (node == 0)
			return;
		trans.setOrigin(btVector3(node->GetPosition().x, node->GetPosition().y, node->GetPosition().z));
		trans.setRotation(btQuaternion(node->GetRotation().x, node->GetRotation().y, node->GetRotation().z, node->GetRotation().w));
	}

	void MotionState::setWorldTransform(const btTransform& trans)
	{
		if (node == 0)
			return;

		node->SetRotation(glm::quat(trans.getRotation().getW(), trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ()));
		node->SetPosition(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	}
}