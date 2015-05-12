//#pragma strict_gs_check( on ) 

#include "Core.h"

using namespace Comet;

int main()
{	
	Comet::Core core;
	core.Init();

	//Setting up deferred shading
	RenderTarget* renderTarget = new RenderTarget(1280, 720);
	renderTarget->CreateImageBuffer("diffuse");
	renderTarget->CreateImageBuffer("position");
	renderTarget->CreateImageBuffer("normal", Texture2D::NORM);
	PostProcess* pp = renderTarget->CreatePostProcess(new Material("data\\shaders\\screen.glsl"));
	pp->SetOutput(core.GetRenderer()->GetRenderTarget());

	
	//The scene
	Comet::Node* camera = core.SceneRoot()->CreateNode();
	camera->AddComponent<Camera>();
	camera->Translate(vec3f(0, 0, 9.0f));
	camera->GetComponent<Camera>()->SetRenderTarget(renderTarget);

	Comet::Node* sky = core.SceneRoot()->CreateNode();
	sky->AddComponent<Mesh>();
	sky->GetComponent<Mesh>()->SetMeshData(Resources().Get<MeshData>("data\\models\\skysphere.fbx"));
	sky->GetComponent<Mesh>()->SetMaterial(new Comet::Material("data\\shaders\\deferred.glsl", "data\\textures\\skydome.tga"));

	Comet::Node* env = core.SceneRoot()->CreateNode();
	env->AddComponent<Mesh>();
	env->GetComponent<Mesh>()->SetMeshData(Resources().Get<MeshData>("data\\models\\env.fbx"));
	env->GetComponent<Mesh>()->SetMaterial(new Comet::Material("data\\shaders\\deferred.glsl", "data\\textures\\177.tga"));

	Comet::Node* character = core.SceneRoot()->CreateNode();
	character->AddComponent<Mesh>();
	character->GetComponent<Mesh>()->SetMeshData(Resources().Get<MeshData>("data\\models\\miku.FBX"));
	character->GetComponent<Mesh>()->SetMaterial(new Comet::Material("data\\shaders\\deferred.glsl", "data\\textures\\roomitems_002_body.png"));
	character->Translate(vec3f(0, 0.0f, 0));
	
	//The animations
	/*
	Animator* anim = character->AddComponent<Animator>();
	Animation* seq = Resources().Get<Animation>("data\\animation\\anim.FBX");
	printf("Loaded %i curves\n", seq->GetCurveCount());
	anim->SetAnimation(seq);

	Animator* skyanim = sky->AddComponent<Animator>();
	Animation* skyseq = new Animation();
	skyseq->SetLength(2);
	Curve* crv = skyseq->NewCurve<RotationCurve>();
	crv->GetCurve(1)->SetKey(0, 0);
	crv->GetCurve(1)->SetKey(2, 6.28f);
	skyanim->SetAnimation(skyseq);*/

	float time = 0;
	double mx = 0, my = 0;
	double dmx = 0, dmy = 0;
	do
	{
		float dt = core.GetDt();
		time += dt;

		//anim->AddTime(15.0f * dt);
		//skyanim->AddTime(dt);

		pp->GetMaterial()->SetParameter("cameraPos", camera->GetBack());
		dmx = mx; dmy = my;
		glfwGetCursorPos(core.GetRenderer()->GetWindow(), &mx, &my);
		dmx -= mx; dmy -= my;
		camera->Rotate((dmx) * 0.002f, vec3f(0, 1, 0), Comet::Node::LOCAL);
		camera->Rotate((dmy) * 0.002f, vec3f(1, 0, 0), Comet::Node::WORLD);
		//glfwSetCursorPos(core.GetRenderer()->GetWindow(), 1280 / 2, 720 / 2);

		//camera->SetPosition(character->GetPosition() + vec3f(0, 1.5f, 0));
		//camera->Translate(camera->GetBack()*9.0f);

		if (glfwGetKey(core.GetRenderer()->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
		{
		camera->Translate(vec3f(0.0f, 0.0f, -5.03f*dt), Node::LOCAL);
		}
		if (glfwGetKey(core.GetRenderer()->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
		{
			camera->Translate(vec3f(0.0f, 0.0f, 5.03f*dt), Node::LOCAL);
		}
		if (glfwGetKey(core.GetRenderer()->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
		{
			camera->Translate(vec3f(-5.03f*dt, 0.0f, 0.0f), Node::LOCAL);
		}
		if (glfwGetKey(core.GetRenderer()->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
		{
			camera->Translate(vec3f(5.03f*dt, 0.0f, 0.0f), Node::LOCAL);
		}
		if (glfwGetKey(core.GetRenderer()->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			camera->Translate(vec3f(0.0f, 5.03f*dt, 0.0f), Node::WORLD);
		}
		if (glfwGetKey(core.GetRenderer()->GetWindow(), GLFW_KEY_C) == GLFW_PRESS)
		{
			camera->Translate(vec3f(0.0f, -5.03f*dt, 0.0f), Node::WORLD);
		}
	} while (core.Update());
	
	return 0;
}