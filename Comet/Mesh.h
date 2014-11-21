#pragma once

#include "Renderable.h"

#include "MeshData.h"

namespace Comet
{

	class Mesh : public Renderable
	{
	public:
		Mesh();
		Mesh(MeshData* data);
		Mesh(MeshData* data, std::string subMeshName);
		~Mesh();

		void SetMeshData(MeshData* md){ meshData = md; }
		void SetSubMesh(std::string name){}
	private:
		
	};

};