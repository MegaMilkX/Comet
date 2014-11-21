#include "TestGame.h"


TestGame::TestGame()
{
}


TestGame::~TestGame()
{
}

void TestGame::Init()
{
	viewport = core->GetRenderer()->CreateViewport();

	glfwSetCursorPos(core->GetRenderer()->GetWindow(), 1280 / 2, 720 / 2);
	glfwGetCursorPos(core->GetRenderer()->GetWindow(), &mx, &my);
	px = mx; py = my;
}

void TestGame::Update()
{
	glfwGetCursorPos(core->GetRenderer()->GetWindow(), &mx, &my);
	viewport->GetCamera()->GetNode()->Rotate((px - mx)*0.005f, glm::vec3(0.0f, 1.0f, 0.0f), 0);
	viewport->GetCamera()->GetNode()->Rotate((py - my)*0.005f, glm::vec3(1.0f, 0.0f, 0.0f), 1);
	//glfwSetCursorPos(renderer->window, 1280 / 2, 720 / 2);
	px = mx; py = my;

	if (glfwGetKey(core->GetRenderer()->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		viewport->GetCamera()->GetNode()->Translate(glm::vec3(0.0f, 0.0f, -0.03f), 0);
	}
	if (glfwGetKey(core->GetRenderer()->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		viewport->GetCamera()->GetNode()->Translate(glm::vec3(0.0f, 0.0f, 0.03f), 0);
	}
	if (glfwGetKey(core->GetRenderer()->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		viewport->GetCamera()->GetNode()->Translate(glm::vec3(-0.03f, 0.0f, 0.0f), 0);
	}
	if (glfwGetKey(core->GetRenderer()->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		viewport->GetCamera()->GetNode()->Translate(glm::vec3(0.03f, 0.0f, 0.0f), 0);
	}
}