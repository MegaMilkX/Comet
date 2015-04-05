#pragma once

#include "GUICursor.h"

#include "GUIElement.h"
#include "GUIText.h"
#include "GUIRect.h"
#include "GUISprite.h"

#include "Camera.h"

namespace Comet
{
	class Core;
	class Viewport;

	class GUI
	{
	public:
		GUI(Viewport* vp);
		~GUI();

		GUIElement* GetRoot(){ return rootElement; }
		GUICursor* GetCursor(){ return cursor; }
		Camera* GetCamera(){ return camera; }
		void Update();

		friend Core;
	private:
		Core* core;
		GUICursor* cursor;
		GUIElement* rootElement;

		Viewport* viewport;
		Camera* camera;
	};

}