#pragma once
#include "Resource.h"
#include "Texture2D.h"

#include <FontRasterizer.h>

#include <map>

namespace Comet
{

	class Font :
		public Resource
	{
	public:
		Font(bool make_default = false);
		~Font();

		void Load(std::string path);
		void MakeDefault()
		{

		}
		void Unload();

		Texture2D* GetTexture(){ return texture2D; }
		GlyphInfo* GetGlyph(unsigned int g);

		float LineHeight(){ return lineHeight; }
	private:
		Texture2D* texture2D;
		FontRasterizer* fr;
		float lineHeight;
	};

}