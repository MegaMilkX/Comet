#include "Core.h"

namespace Comet
{
	Core* Core::instance;

	Core::Core()
	{
		ResMan::Initialize();

		instance = this;

		game = 0;
		renderer = new Renderer();

		userConsole = new UserConsole();
	}


	Core::~Core()
	{
		delete userConsole;

		if (game)
			delete game;
		delete renderer;

		ResMan::Destroy();
	}

	void Core::Init()											//Инициализация здесь ---------------------------------------------------
	{
		renderer->Init();
		printf("[OK]Ready.\n");
	}

	void Core::Reset()											//Ресет используется для перезапуска И при старте -----------------------
	{
		LoadScene("whatever");

		_loop();
	}

	void Core::_loop()											//
	{
		do
		{
			if (game)
				game->Update();
		}
		while (renderer->Update());
	}

	void Core::LoadScene(std::string path)
	{
		Node* nodeCamera = renderer->rootNode->CreateNode();
		Node* node = renderer->rootNode->CreateNode();
		Node* node2 = renderer->rootNode->CreateNode();

		Camera* cam = new Camera();
		cam->Perspective(95.0f, 1280.0f / 720.0f, 0.001f, 1000.0f);
		cam->SetViewport(*(renderer->viewports.begin()));

		nodeCamera->Attach(cam);
		nodeCamera->Translate(glm::vec3(0.2f, 0.3f, 3.0f), 0);

		Light* light = new Light();
		//Mesh* mesh = new Mesh(ResMan::GetInstance()->GetResource<MeshData>("data\\models\\test.xyz"));
		Mesh* mesh2 = new Mesh(ResMan::GetInstance()->GetResource<MeshData>("data\\models\\test2.xyz"));
		mesh2->SetTexture2D(ResMan::GetInstance()->GetResource<Texture2D>("data\\textures\\test2.tga"), 0);
		//node->Attach(mesh);
		node->Attach(light);
		node2->Attach(mesh2);
		node2->Translate(glm::vec3(0.0f, 0.0f, 0.0f), 0);
	}

};