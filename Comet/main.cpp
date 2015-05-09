//#pragma strict_gs_check( on ) 

#include "Core.h"

int main()
{	
	Comet::Core core;
	core.Init();

	Comet::Node* object = core.SceneRoot()->CreateNode();
	object->AddComponent(new Comet::Camera());
	object->Translate(vec3f(0, 0, 9.0f));

	Comet::Node* ball = core.SceneRoot()->CreateNode();
	ball->AddComponent(new Comet::Mesh("data\\models\\sphere.fbx"));
	ball->GetComponent<Comet::Mesh>()->SetMaterial(new Comet::Material("data\\shaders\\terrain.glsl", "data\\textures\\skydome.tga"));


	do
	{
		ball->Rotate(0.01f, vec3f(0, 1, 0));
	} while (core.Update());
	
	return 0;
}