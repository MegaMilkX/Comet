#pragma once
#include "Resource.h"

#include <tga.h>
#include <SOIL.h>

namespace Comet
{

	class Texture2D : public Resource
	{
	public:
		enum TEXFMT
		{
			RGBA,
			RGB,
			DEPTH16,
			DEPTH24,
			DEPTH32
		};
		Texture2D();
		Texture2D(unsigned int w, unsigned int h, TEXFMT fmt);
		~Texture2D();

		void Load(std::string path);
		void LoadFromMemory(unsigned char* data, unsigned int w, unsigned int h);
		void Unload();

		unsigned int GetTextureId(){ return texId; }
		unsigned int GetWidth(){ return width; }
		unsigned int GetHeight(){ return height; }
		unsigned char GetBPP(){ return bpp; }

		void SetTextureId(unsigned int id){ texId = id; }
		void SetDimensions(unsigned int w, unsigned int h){ width = w; height = h; }
	private:
		unsigned int texId;
		unsigned int width, height;
		unsigned char bpp;
	};

};