#include "WorldMapScene.h"

#include "AdventureGame.h"

#include "ResMan.h"

WorldMapScene::WorldMapScene(AdventureGame* game)
{
	this->game = game;
	tileMap = new WorldTileMap(game);
}

WorldMapScene::~WorldMapScene()
{
	delete tileMap;
}


void WorldMapScene::Init()
{
	//Создание тайлов карты, нодов на карте, еще какой-нибудь хуйни
	tileMap->Init(6, 5);

	Comet::Node* landNode = Comet::Core::GetInstance()->GetRenderer()->GetRoot()->CreateNode();
	//Comet::Landscape* landMesh = new Comet::Landscape("data\\land\\land");
	//landMesh->SetLandscapeSize(3, 2);
	//landMesh->SetTileSize(17);
	//landMesh->Rebuild();
	//landNode->Attach(landMesh);

	Comet::Material* landMat = new Comet::Material();
	landMat->SetShader("data\\shaders\\test.glsl");
	//landMesh->SetMaterial(landMat);
	landMat->SetTexture2D("data\\textures\\test.tga", 0);

	//Камера и личный вьюпорт для сцены карты
	//viewport = game->GetRenderer()->CreateViewport();
	//СОЗДАНИЕ ВЬЮПОРТОВ ПОКА НЕ РАБОТАЕТ, БЕРИ ДЕФОЛТНЫЙ
	//viewport = *(game->GetRenderer()->GetViewports().begin());
	camera = new Comet::Camera();
	camera->Perspective(95.0f, 1280.0f / 720.0f, 0.001f, 1000.0f);
	//camera->SetViewport(viewport);

	nodeCamera = game->GetRenderer()->GetRoot()->CreateNode();
	nodeCamera->SetPosition(vec3f(0.0f, 0.0f, 270.0f));
	//nodeCamera->Rotate(0.8f, glm::vec3(1, 0, 0), 0);
	nodeCamera->Attach(camera);
}

void WorldMapScene::OnSwitch(bool on)
{
	//true
	//Включить все ноды
	//false
	//Выключить все ноды

	//Погасить вьюпорт

	//хули сложного
}

void WorldMapScene::Update()
{
	if (glfwGetKey(game->GetRenderer()->GetWindow(), GLFW_KEY_W))
	{
		nodeCamera->Translate(vec3f(0, 50.0f*game->GetDt(), 0.0f), Comet::Node::WORLD);
	}

	if (glfwGetKey(game->GetRenderer()->GetWindow(), GLFW_KEY_A))
	{
		nodeCamera->Translate(vec3f(-50.0f*game->GetDt(), 0.0f, 0.0f), Comet::Node::LOCAL);
	}

	if (glfwGetKey(game->GetRenderer()->GetWindow(), GLFW_KEY_S))
	{
		nodeCamera->Translate(vec3f(0, -50.0f*game->GetDt(), 0.0f), Comet::Node::WORLD);
	}

	if (glfwGetKey(game->GetRenderer()->GetWindow(), GLFW_KEY_D))
	{
		nodeCamera->Translate(vec3f(50.0f*game->GetDt(), 0.0f, 0.0f), Comet::Node::LOCAL);
	}
}