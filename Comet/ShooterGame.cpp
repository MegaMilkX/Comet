#include "ShooterGame.h"

using namespace Comet;

ShooterGame::ShooterGame()
{
	
}


ShooterGame::~ShooterGame()
{
}

void ShooterGame::Init()
{
	Core::Init();
	PostInit();
}

void ShooterGame::PostInit()
{
	//Rendering pipeline setup?
	//TODO
	//screenMat = new Material("data\\shaders\\screen.glsl");
	//renderer->GetRootRenderPass()->SetMaterial(screenMat);
	//RenderPass* pass = renderer->GetRootRenderPass()->CreatePass();
	//pass->CreateImageBuffer("normals");

	//Render pipeline with render targets
	renderer->GetRenderTarget();
	RenderTarget* renderTarget = new RenderTarget(1280, 720);
	renderTarget->CreateImageBuffer("diffuse");
	
	//Viewport
	Viewport* viewport = renderer->DefaultViewport();
	viewport->SetColor(0.3f, 0.1f, 0.2f);
	Viewport* vp0 = renderer->CreateViewport();
	Viewport* vp = renderer->CreateViewport();
	vp->SetRect(0, 0, 0.3f, 0.3f);
	vp->SetColor(0.0f, 0.0f, 0.0f);

	//Камера
	camera_ = CreateEntity();
	camera_->AddComponent(renderer->GetRoot()->CreateNode());
	camera_->AddComponent(new Camera());
	camera_->GetComponent<Camera>()->Perspective(95.0f, 1280.0f / 720.0f, 0.01f, 1000.0f);
	camera_->GetComponent<Camera>()->SetViewport(vp);
	//camera_->GetComponent<Camera>()->SetRenderTarget(renderTarget);
	camera_->AddComponent(physics->CreateRigidBody(new btSphereShape(1), 0, true));

	//моделька персонажа
	Node* node = renderer->GetRoot()->CreateNode();
	node->SetPosition(glm::vec3(0, 3.1f, 0));
	Mesh* mesh = new Mesh("data\\models\\girl.xyz");
	node->Attach(mesh);

	Material* mat = new Material();
	mat->SetShader("data\\shaders\\terrain.glsl");
	mat->SetTexture2D("data\\textures\\test2.tga", 0);
	mesh->SetMaterial(mat);
	//-------------------


	
	


	//Пол
	Entity* ground = CreateEntity();
	ground->AddComponent(renderer->GetRoot()->CreateNode());
	ground->AddComponent(new Mesh(renderer->GetMeshDataPrimitive("plane")));
	ground->GetComponent<Mesh>()->SetMaterial(new Material("data\\shaders\\sphere.glsl", "data\\textures\\WoodFine0007_M.jpg"));
	ground->AddComponent(physics->CreateRigidBody(new btBoxShape(btVector3(25, 0, 25)), 0));
	ground->GetComponent<Node>()->Scale(glm::vec3(50, 50, 1));
	ground->GetComponent<Node>()->Rotate(-3.14f / 2.0f, glm::vec3(1, 0, 0), 0);

	//
	this->ReadGraphFile("data\\test.graph", renderer->GetRoot());

	//Marching stars
	Material* LandMaterial = new Material("data\\shaders\\marchingstar.glsl", "data\\textures\\Die-Sargprinzessin-1-4_.png");

	Entity* tetra = CreateEntity();
	tetra->AddComponent(renderer->GetRoot()->CreateNode());
	tetra->AddComponent(new VoxelVolumeMesh(16, 16, 16));
	tetra->GetComponent<VoxelVolumeMesh>()->SetMaterial(LandMaterial);
	//tetra->GetComponent<VoxelVolumeMesh>()->GetMaterial()->SetPolygonMode(Material::WIRE);
	//tetra->GetComponent<VoxelVolumeMesh>()->GetMeshData()->SetPrimitiveType(MeshData::POINT);
	volumeMesh = tetra->GetComponent<VoxelVolumeMesh>();
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
		sphere->GetComponent<Mesh>()->SetMaterial(new Material("data\\shaders\\sphere.glsl", "data\\textures\\2014-11-20-688057.jpeg"));
		btCollisionShape* shape = new btSphereShape(1);
		btCollisionShape* cyllinder = new btCylinderShape(btVector3(1, 1, 1.5f));
		btCollisionShape* capsule = new btCapsuleShape(1, 0);
		sphere->AddComponent(physics->CreateRigidBody(capsule));
		sphere->GetComponent<RigidBody>()->SetPosition(camera_->GetComponent<Node>()->GetPosition() - camera_->GetComponent<Node>()->GetBack()*2.0f);
		sphere->GetComponent<RigidBody>()->GetBody()->applyCentralImpulse(btVector3(-camera_->GetComponent<Node>()->GetBack().x, -camera_->GetComponent<Node>()->GetBack().y, -camera_->GetComponent<Node>()->GetBack().z)*20.0f);
	}

	disp += 1.0f * GetDt();
	volumeMesh->_MoveNoise(disp, 0, 0);

	return Core::_postUpdate();
}