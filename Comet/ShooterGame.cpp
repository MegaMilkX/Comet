#include "ShooterGame.h"

using namespace Comet;

ShooterGame::ShooterGame()
{
	world = 0;
}


ShooterGame::~ShooterGame()
{
	if (world)
		delete world;
}

void ShooterGame::Init()
{
	Core::Init();
	PostInit();
}

void ShooterGame::PostInit()
{
	//Render pipeline with render targets
	RenderTarget* renderTarget = new RenderTarget(1280, 720);
	renderTarget->CreateImageBuffer("diffuse");
	renderTarget->CreateImageBuffer("position");
	renderTarget->CreateImageBuffer("normal", Texture2D::NORM);

	pp = renderTarget->CreatePostProcess(new Material("data\\shaders\\screen.glsl"));
	pp->SetOutput(renderer->GetRenderTarget());

	//Камера
	camera_ = CreateEntity();
	camera_->AddComponent(renderer->GetRoot()->CreateNode());
	camera_->AddComponent(new Camera());
	camera_->GetComponent<Camera>()->Perspective(95.0f, 1280.0f / 720.0f, 0.01f, 100.0f);
	camera_->AddComponent(physics->CreateRigidBody(new btSphereShape(1), 0, true));
	camera_->GetComponent<Camera>()->SetRenderTarget(renderTarget);

	
	/////////////////////////////////////
	//TODO: Initialize the game here
	world = new VoxelWorld(this);

	/////////////////////////////////////

	
	//моделька персонажа
	Node* node = renderer->GetRoot()->CreateNode();
	node->SetPosition(glm::vec3(0, 3.1f, 0));
	Mesh* mesh = new Mesh("data\\models\\girl.xyz");
	node->Attach(mesh);

	Material* mat = new Material();
	mat->SetShader("data\\shaders\\deferred.glsl");
	mat->SetTexture2D("data\\textures\\test2.tga", 0);
	//mat->SetTexture2D(renderTarget->GetImageBuffer("diffuse"), 0);
	//mat->SetTexture2D(renderTarget->GetDepthBuffer(), 0);
	mesh->SetMaterial(mat);
	//-------------------

	//Пол
	Entity* ground = CreateEntity();
	ground->AddComponent(renderer->GetRoot()->CreateNode());
	ground->AddComponent(new Mesh(renderer->GetMeshDataPrimitive("plane")));
	ground->GetComponent<Mesh>()->SetMaterial(new Material("data\\shaders\\deferrednormal.glsl", "data\\textures\\177.tga"));
	ground->GetComponent<Mesh>()->GetMaterial()->SetTexture2D("data\\textures\\177_norm.tga", 1);
	ground->AddComponent(physics->CreateRigidBody(new btBoxShape(btVector3(25, 0, 25)), 0));
	ground->GetComponent<Node>()->Scale(glm::vec3(50, 50, 1));
	ground->GetComponent<Node>()->Rotate(-3.14f / 2.0f, glm::vec3(1, 0, 0), 0);

	//Hierarchy loading
	//this->ReadGraphFile("data\\test.graph", renderer->GetRoot());

	//Marching cubes
	/*
	Material* LandMaterial = new Material("data\\shaders\\deferredmarchingcubes.glsl", "data\\textures\\GrassDead0102_1_S.jpg");
	LandMaterial->SetTexture2D("data\\textures\\Grass0026_1_S.jpg", 1);

	Entity* tetra = CreateEntity();
	tetra->AddComponent(renderer->GetRoot()->CreateNode());
	tetra->AddComponent(new VoxelVolumeMesh(16, 16, 16));
	tetra->GetComponent<VoxelVolumeMesh>()->SetMaterial(LandMaterial);
	volumeMesh = tetra->GetComponent<VoxelVolumeMesh>();
	*/
}

void ShooterGame::Start()
{
}

bool ShooterGame::Update()
{
	Core::_preUpdate();

	glfwGetCursorPos(renderer->GetWindow(), &mx, &my);
	camera_->GetComponent<Node>()->Rotate((1280 / 2 - mx)*0.003f, glm::vec3(0.0f, 1.0f, 0.0f), 0);
	camera_->GetComponent<Node>()->Rotate((720 / 2 - my)*0.003f, glm::vec3(1.0f, 0.0f, 0.0f), 1);
	
	glfwSetCursorPos(renderer->GetWindow(), 1280 / 2, 720 / 2);

	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_->GetComponent<Node>()->Translate(glm::vec3(0.0f, 0.0f, -5.03f*dt), 0);
	}
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_->GetComponent<Node>()->Translate(glm::vec3(0.0f, 0.0f, 5.03f*dt), 0);
	}
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_->GetComponent<Node>()->Translate(glm::vec3(-5.03f*dt, 0.0f, 0.0f), 0);
	}
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_->GetComponent<Node>()->Translate(glm::vec3(5.03f*dt, 0.0f, 0.0f), 0);
	}
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera_->GetComponent<Node>()->Translate(glm::vec3(0.0f, 5.03f*dt, 0.0f), 1);
	}
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_C) == GLFW_PRESS)
	{
		camera_->GetComponent<Node>()->Translate(glm::vec3(0.0f, -5.03f*dt, 0.0f), 1);
	}

	if (glfwGetMouseButton(renderer->GetWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		Entity* sphere = CreateEntity();
		sphere->AddComponent(renderer->GetRoot()->CreateNode());
		sphere->AddComponent(new Mesh("data\\models\\sphere.xyz"));
		sphere->GetComponent<Mesh>()->SetMaterial(new Material("data\\shaders\\deferred.glsl", "data\\textures\\2014-11-20-688057.jpeg"));
		btCollisionShape* shape = new btSphereShape(1);
		btCollisionShape* cyllinder = new btCylinderShape(btVector3(1, 1, 1.5f));
		btCollisionShape* capsule = new btCapsuleShape(1, 0);
		sphere->AddComponent(physics->CreateRigidBody(capsule));
		sphere->GetComponent<RigidBody>()->SetPosition(camera_->GetComponent<Node>()->GetPosition() - camera_->GetComponent<Node>()->GetBack()*2.0f);
		sphere->GetComponent<RigidBody>()->GetBody()->applyCentralImpulse(btVector3(-camera_->GetComponent<Node>()->GetBack().x, -camera_->GetComponent<Node>()->GetBack().y, -camera_->GetComponent<Node>()->GetBack().z)*20.0f);
	}

	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
		pp->GetMaterial()->SetParameter("output", 0);
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
		pp->GetMaterial()->SetParameter("output", 1);
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_3) == GLFW_PRESS)
		pp->GetMaterial()->SetParameter("output", 2);
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_4) == GLFW_PRESS)
		pp->GetMaterial()->SetParameter("output", 3);

	disp += 1.0f * GetDt();
	//volumeMesh->_MoveNoise(disp, 0, 0);
	pp->GetMaterial()->SetParameter("cameraPos", camera_->GetComponent<Camera>()->GetNode()->GetBack());

	world->Update(camera_->GetComponent<Node>()->GetPosition());

	return Core::_postUpdate();
}