#pragma once

#include "RenderObject.h"

#include "MeshData.h"
#include "Texture2D.h"

#define VATTR_POS 0x0001
#define VATTR_UVW 0x0002
#define VATTR_NOR 0x0004
#define VATTR_COL 0x0008

namespace Comet
{

	class Renderable : public RenderObject
	{
	public:
		Renderable();
		virtual ~Renderable();

		MeshData* GetMeshData(){ return meshData; }
		std::vector<Texture2D*> GetTextures(){ return textures; }

		void SetTexture2D(Texture2D* tex, unsigned char layer);
	protected:
		MeshData* meshData; //Переделать на смарт поинтер. Иначе пиздец
		std::vector<Texture2D*> textures; //Это тоже
	};

};