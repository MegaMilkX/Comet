#include "Font.h"

#include "Renderer.h"
#include "ResMan.h"

#include <Windows.h>

namespace Comet
{

	Font::Font(bool make_default)
	{
		fr = 0;
	}


	Font::~Font()
	{
		Unload();
	}

	void Font::Load(std::string path)
	{
		fr = new FontRasterizer();

		fr->ReadFontFile(path);

		fr->AddGlyphRange(0, 128);
		fr->AddGlyphRange(0x410, 0x44f);

		int sz = 16;
		if (!(Resources().GetParamStack()->empty()))
		{
			sz = Resources().GetParamStack()->top();
			Resources().GetParamStack()->pop();
		}

		fr->SetSize(sz);
		fr->Rasterize();

		Bitmap bmp = fr->GetBitmapData();
		texture2D = new Texture2D();
		texture2D->LoadFromMemory((unsigned char*)bmp.data, bmp.w, bmp.h);

		lineHeight = fr->GetLineHeight();

		isReady = true;
	}

	void Font::Unload()
	{
		if (texture2D)
			delete texture2D;
		if (fr)
			delete fr;
	}

	GlyphInfo* Font::GetGlyph(unsigned int g)
	{
		return (fr->GetGlyph(g));
	}
}