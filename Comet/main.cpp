//#pragma strict_gs_check( on ) 

#include "AdventureGame.h"
#include "ShooterGame.h"

#include <math3f.h>

int main()
{/*
	ShooterGame game;
	game.Init();

	while (game.Update());*/
	
	
	Comet::Core core;
	core.Init();
	/*
	Comet::Entity* entity = core.CreateEntity();
	entity->AddComponent(core.GetRenderer()->GetRoot()->CreateNode());
	entity->AddComponent(new Comet::Camera());
	entity->GetTransform()->Translate(vec3f(0, 0, 13.0f));
	
	Comet::Entity* ball = core.CreateEntity();
	ball->AddComponent(core.GetRenderer()->GetRoot()->CreateNode());
	ball->AddComponent(new Comet::Mesh("data\\models\\sphere.fbx"));
	ball->GetComponent<Comet::Mesh>()->SetMaterial(new Comet::Material("data\\shaders\\terrain.glsl", "data\\textures\\Die-Sargprinzessin-1-4_.png"));
	*/

	Comet::Node* object = core.SceneRoot()->CreateNode();
	object->AddComponent(new Comet::Camera());
	object->Translate(vec3f(0, 0, 9.0f));

	Comet::Node* ball = core.SceneRoot()->CreateNode();
	ball->AddComponent(new Comet::Mesh("data\\models\\sphere.fbx"));
	ball->GetRenderable()->SetMaterial(new Comet::Material("data\\shaders\\terrain.glsl", "data\\textures\\test.tga"));


	do
	{

	} while (core.Update());
	
	return 0;
}