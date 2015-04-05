#include "Mesh.h"

#include "Core.h"

namespace Comet
{

	Mesh::Mesh()
	{
		
		
	}

	Mesh::Mesh(std::string resname)
	{
		SetMeshData(resname);
		
	}

	Mesh::Mesh(MeshData* data)
	{
		meshData = data;
	}
	Mesh::Mesh(MeshData* data, std::string subMeshName)
	{
		meshData = data;
	}

	Mesh::~Mesh()
	{
	}

};