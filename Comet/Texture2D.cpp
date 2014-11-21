#include "Texture2D.h"

#include "Renderer.h"

namespace Comet
{

	Texture2D::Texture2D()
	{
	}


	Texture2D::~Texture2D()
	{
		Unload();
	}

	void Texture2D::Load(std::string path)
	{
		TGA* file;
		TGAData tgaData;
		file = TGAOpen((char*)path.c_str(), "r");

		if (!file || file->last != TGA_OK)
		{
			printf("[ER]\"%s\" not found!", path.c_str());
			return;
		}

		tgaData.flags = TGA_IMAGE_DATA | TGA_COLOR_MAP | TGA_IMAGE_INFO | TGA_RGB;
		if (TGAReadImage(file, &tgaData) != TGA_OK)
		{
			TGAClose(file);
			return;
		}
		//tgaData->img_data = (tbyte*)new char[file->hdr.width * file->hdr.height * 3];

		//попенгл
		glGenBuffers(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, file->hdr.width, file->hdr.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, tgaData.img_data);

		//
		TGAClose(file);
	}

	void Texture2D::Unload()
	{
		glDeleteBuffers(1, &texId);
	}
};