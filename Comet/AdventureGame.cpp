#include "AdventureGame.h"

#include <sstream>

AdventureGame::AdventureGame()
{
	currentScene = 0;
	worldMap = new WorldMapScene(this);
	party = new Party();
}


AdventureGame::~AdventureGame()
{
	delete party;
	delete worldMap;
}

void AdventureGame::Init()
{
	Core::Init();
	PostInit();
	printf("Ready.\n");
}

void AdventureGame::PostInit()
{
	//TODO: Всякие иниты при запуске игры, загрузка ресурсов, подготовка объектов
	worldMap->Init();
	currentScene = worldMap;

	//text = new Comet::TextRenderable("What's with the upper case?", "data\\fonts\\Magic.ttf", 100);
	Comet::Node* textNode = renderer->GetRoot()->CreateNode();
	textNode->SetPosition(glm::vec3(0, 150.0f, 0));
	//textNode->Attach(text);
	

	/*
	Comet::GUIElement* charFrameContainer = gui->GetRoot()->CreateElement<Comet::GUIElement>();
	charFrameContainer->SetRect(8, 8, 168, 72);
	charFrameContainer->SetName("characterFrame");
	charFrameContainer->SetAlign(Comet::TOP | Comet::LEFT);
	
		Comet::GUIRect* blackBG = charFrameContainer->CreateElement<Comet::GUIRect>();
		blackBG->SetColor(0.0f, 0.0f, 0.0f, 0.6f);
		blackBG->SetRect(38, 6, 130, 64);
		blackBG->SetName("blackBG");
		
			Comet::GUIText* charName = blackBG->CreateElement<Comet::GUIText>();
			charName->SetText("Gilligan");
			charName->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			charName->SetName("charName");
			charName->SetRect(38, 2, 64, 64);
			charName->SetAlign(Comet::TOP | Comet::LEFT);

			Comet::GUIRect* hpBar = blackBG->CreateElement<Comet::GUIRect>();
			hpBar->SetColor(0.8f, 0.1f, 0.1f, 1.0f);
			hpBar->SetName("Health Bar");
			hpBar->SetRect(38, 25, 88, 8);
			hpBar->SetAlign(Comet::TOP | Comet::LEFT);

			Comet::GUISprite* portraitSprite = blackBG->CreateElement<Comet::GUISprite>();
			portraitSprite->SetTexture("data\\textures\\portrait.tga");
			portraitSprite->SetRect(-31, 0, 64, 64);
			portraitSprite->SetName("portraitSprite");

				Comet::GUISprite* portraitFrame = portraitSprite->CreateElement<Comet::GUISprite>();
				portraitFrame->SetTexture("data\\textures\\portrait_frame.tga");
				portraitFrame->SetRect(-2, -2, 68, 68);
				portraitFrame->SetName("portraitFrame");*/
}

bool AdventureGame::Update()
{
	Core::_preUpdate();

	currentScene->Update();

	std::stringstream ss;
	ss.clear();
	//ss << "Magic>: " << glfwGetTime() << " " << gui->GetCursor()->Y();
	//ss << "Magic>: " << gui->GetCursor()->X() << " " << gui->GetCursor()->Y();
	/*
	if (gui->GetCursor()->GetFocusElement())
	{
		ss << gui->GetCursor()->GetFocusElement()->GetName();
	}
	else
	{
		ss << "empty";
	}*/
	//text->GetMaterial()->SetParameter("time", (float)glfwGetTime());
	//text->SetText(ss.str());

	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
	{
	}
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
	{
	}
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_3) == GLFW_PRESS)
	{
	}
	if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_4) == GLFW_PRESS)
	{
	}

	return _postUpdate();
}
