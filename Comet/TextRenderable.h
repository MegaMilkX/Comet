#pragma once

#include "Renderable.h"
#include "ResMan.h"

namespace Comet
{

	class TextRenderable : public Renderable
	{
	public:
		TextRenderable();
		TextRenderable(std::string str);
		TextRenderable(std::string str, int sz);
		TextRenderable(std::string str, std::string font, int sz);
		TextRenderable(std::string str, std::string font, int sz, std::string shader);
		~TextRenderable();

		void SetText(std::string str);

		glm::vec4 GetBoundingRect();

		friend Font;
	private:
		std::string text;
		Comet::Font* font;
		glm::vec4 bndRect;
	};

}