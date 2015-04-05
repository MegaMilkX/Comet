#include "Renderable.h"

#include "Core.h"

namespace Comet
{

	Renderable::Renderable()
	{
		meshData = 0;
		textures.clear();

		Core::GetInstance()->GetRenderer()->_regRenderable(this);
	}


	Renderable::~Renderable()
	{
	}

	void Renderable::SetMeshData(std::string resname)
	{
		meshData = ResMan::GetInstance()->GetResource<MeshData>(resname);
	}

	void Renderable::SetMaterial(std::string resname)
	{
		mat = ResMan::GetInstance()->GetResource<Material>(resname);
	}

	void Renderable::SetTexture2D(Texture2D* tex, unsigned char layer)
	{
		if (textures.size() >= layer)
		{
			textures.resize(layer + 1);
		}

		textures[layer] = tex;
	}


	//
	
};