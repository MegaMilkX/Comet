#include "BaseGame.h"

namespace Comet
{

	BaseGame::BaseGame()
	{

	}


	BaseGame::~BaseGame()
	{
		
	}

	void BaseGame::PostInit()
	{
		glfwSetCursorPos(renderer->GetWindow(), 1280 / 2, 720 / 2);
		glfwGetCursorPos(renderer->GetWindow(), &mx, &my);
		px = mx; py = my;

		//Установка сцены
		Node* nodeCamera = renderer->GetRoot()->CreateNode();
		Node* node = renderer->GetRoot()->CreateNode();
		node2 = renderer->GetRoot()->CreateNode();

		cam = new Camera();
		cam->Perspective(95.0f, 1280.0f / 720.0f, 0.001f, 1000.0f);

		nodeCamera->Attach(cam);
		nodeCamera->Translate(glm::vec3(0.2f, 0.3f, 3.0f), 0);

		Light* light = new Light();
		//Mesh* mesh = new Mesh(ResMan::GetInstance()->GetResource<MeshData>("data\\models\\test.xyz"));
		Mesh* mesh2 = new Mesh(ResMan::GetInstance()->GetResource<MeshData>("data\\models\\test2.xyz"));
		mesh2->SetTexture2D(ResMan::GetInstance()->GetResource<Texture2D>("data\\textures\\test2.tga"), 0);
		mesh2->SetMaterial(ResMan::GetInstance()->GetResource<Material>("data\\materials\\test.mat"));

		//node->Attach(mesh);
		node->Attach(light);
		node2->Attach(mesh2);
		node2->Translate(glm::vec3(0.0f, 0.0f, 0.0f), 0);
		//node2->Rotate(0.3f, glm::vec3(1, 0, 0), 0);
	}

	bool BaseGame::Update()
	{
		glfwGetCursorPos(renderer->GetWindow(), &mx, &my);
		cam->GetNode()->Rotate((px - mx)*0.005f, glm::vec3(0.0f, 1.0f, 0.0f), 0);
		cam->GetNode()->Rotate((py - my)*0.005f, glm::vec3(1.0f, 0.0f, 0.0f), 1);
		//glfwSetCursorPos(renderer->window, 1280 / 2, 720 / 2);
		px = mx; py = my;

		if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
		{
			cam->GetNode()->Translate(glm::vec3(0.0f, 0.0f, -5.03f*dt), 0);
		}
		if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
		{
			cam->GetNode()->Translate(glm::vec3(0.0f, 0.0f, 5.03f*dt), 0);
		}
		if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
		{
			cam->GetNode()->Translate(glm::vec3(-5.03f*dt, 0.0f, 0.0f), 0);
		}
		if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
		{
			cam->GetNode()->Translate(glm::vec3(5.03f*dt, 0.0f, 0.0f), 0);
		}

		node2->Rotate(3.0f * dt, glm::vec3(0, 1, 0), 0);

		return true;
	}
}