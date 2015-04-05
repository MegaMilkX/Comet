#include "Physics.h"

namespace Comet
{

	Physics::Physics()
	{
		broadphase = new btDbvtBroadphase();
		collisionConfig = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfig);
		solver = new btSequentialImpulseConstraintSolver();

		world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
		world->setGravity(btVector3(0, -10, 0));
	}


	Physics::~Physics()
	{
		delete world;
		delete solver;
		delete dispatcher;
		delete collisionConfig;
		delete broadphase;
	}

	void Physics::Update()
	{
		world->stepSimulation(1 / 60.0f, 10);
	}

	RigidBody* Physics::CreateRigidBody(btCollisionShape* shape, float mass, bool isKinematic)
	{
		if (shape == 0)
			shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
		
		RigidBody* rb = new RigidBody(this, shape, mass, isKinematic);
		return rb;
	}

}