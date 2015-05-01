#include "Player.h"

using namespace Comet;

Player::Player(Core* core)
{
	entity = core->CreateEntity();
	entity->AddComponent(core->GetRenderer()->GetRoot()->CreateNode());
	entity->AddComponent(core->GetPhysics()->CreateRigidBody(new btCapsuleShape(1, 2), 0, true));
}


Player::~Player()
{
	delete entity;
}

void Player::Update()
{

}