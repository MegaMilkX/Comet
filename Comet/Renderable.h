#pragma once

#include "RenderObject.h"

#include "MeshData.h"
#include "Texture2D.h"
#include "Material.h"

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
		Material* GetMaterial(){ return mat; }
		std::vector<Texture2D*> GetTextures(){ return textures; }

		void SetMeshData(std::string resname);
		void SetMeshData(MeshData* md){ meshData = md; }
		void SetTexture2D(Texture2D* tex, unsigned char layer);
		void SetMaterial(std::string resname);
		void SetMaterial(Material* mat){ this->mat = mat; }

		bool IsRenderable(){ return true; }


		
	protected:
		MeshData* meshData; //Переделать на смарт поинтер. Иначе пиздец
		std::vector<Texture2D*> textures; //Это тоже
		Material* mat; //И это конечно
	};

};