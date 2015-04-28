#include "GUI.h"
#include "Core.h"

namespace Comet
{

	GUI::GUI(Viewport* vp)
	{
		cursor = new GUICursor();
		rootElement = new GUIElement(0);
		rootElement->SetRect(0, 0, 1280, 720);

		viewport = vp;

		//camera = new Camera();
		//camera->Ortho(0, vp->GetWidth(), 0, vp->GetHeight(), -1, 1);
	}


	GUI::~GUI()
	{
		//delete camera;

		delete rootElement;
		delete cursor;
	}

	void GUI::Update()
	{
		//Следим за курсором здесь
		cursor->Reset();

		//glfwGetCursorPos(viewport->GetRenderer()->GetWindow(), &(cursor->x), &(cursor->y));
		rootElement->Update(cursor);

		cursor->Update();
	}

}