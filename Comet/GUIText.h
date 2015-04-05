#pragma once
#include "GUIElement.h"

#include "TextRenderable.h"

namespace Comet
{

	class GUIText :
		public GUIElement
	{
	public:
		GUIText(GUIElement* prnt);
		~GUIText();

		void SetZIndex(int zi);
		void SetText(std::string str);
		void SetRect(float x, float y, float w, float h){ rect.x = x; rect.y = y; }
	private:
		std::string text;
		TextRenderable* renderable;
	};

}