#pragma once
#include "GUIRect.h"

namespace Comet
{

	class GUISprite :
		public GUIRect
	{
	public:
		GUISprite(GUIElement* prnt);
		~GUISprite();

		void SetTexture(std::string str);
	protected:
	};

}