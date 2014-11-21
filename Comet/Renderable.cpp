#include "Renderable.h"

namespace Comet
{

	Renderable::Renderable()
	{
		meshData = 0;
		textures.clear();
	}


	Renderable::~Renderable()
	{
	}

	void Renderable::SetTexture2D(Texture2D* tex, unsigned char layer)
	{
		if (textures.size() >= layer)
		{
			textures.resize(layer + 1);
		}

		textures[layer] = tex;
	}

};