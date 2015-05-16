#include "Sprite.h"

namespace Comet
{
	MeshData* _getQuadMeshData()
	{
		static QuadMeshData qmd;
		return &qmd;
	}
	
	Material* _getSpriteMaterial()
	{
		static Material mat;
		return &mat;
	}

	Sprite::Sprite()
	{
		meshData = _getQuadMeshData();
		mat = _getSpriteMaterial();
	}


	Sprite::~Sprite()
	{
	}

}