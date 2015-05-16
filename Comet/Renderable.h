#pragma once

#include "RenderObject.h"

#include "MeshData.h"
#include "Texture2D.h"
#include "Material.h"
#include "Camera.h"

#include "ResMan.h"

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
		virtual			~Renderable();

		virtual MeshData*		GetMeshData() const { return meshData.get(); }
		virtual Material*		GetMaterial() const { return mat.get(); }

		void			SetMeshData(std::string resname);
		void			SetMeshData(MeshData* md){ meshData = md; }
		void			SetMaterial(std::string resname);
		void			SetMaterial(Material* mat){ this->mat = mat; }
		bool			IsRenderable() const { return true; }

		virtual void	Render(const Camera *const cam);
	protected:
		res_ptr<MeshData>		meshData; //Переделать на смарт поинтер. Иначе пиздец
		res_ptr<Material>		mat; //И это конечно
	};

};