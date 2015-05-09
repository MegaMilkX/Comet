#pragma once

#include "Renderable.h"

#include "MeshData.h"



namespace Comet
{

	class Mesh : public Renderable
	{
	public:
		Mesh();
		Mesh(std::string resname);
		Mesh(MeshData* data);
		Mesh(MeshData* data, std::string subMeshName);
		~Mesh();

	private:
		
	};

};