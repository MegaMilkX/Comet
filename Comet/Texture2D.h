#pragma once
#include "Resource.h"

#include <tga.h>

namespace Comet
{

	class Texture2D : public Resource
	{
	public:
		Texture2D();
		~Texture2D();

		void Load(std::string path);
		void Unload();

		unsigned int GetTextureId(){ return texId; }
		unsigned int GetWidth(){ return width; }
		unsigned int GetHeight(){ return height; }
		unsigned char GetBPP(){ return bpp; }
	private:
		unsigned int texId;
		unsigned int width, height;
		unsigned char bpp;
	};

};