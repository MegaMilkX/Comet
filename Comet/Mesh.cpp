#include "Mesh.h"

#include "Core.h"

namespace Comet
{

	Mesh::Mesh()
	{
		Core::GetInstance()->GetRenderer()->_regRenderable(this);
		
	}

	Mesh::Mesh(MeshData* data)
	{
		Core::GetInstance()->GetRenderer()->_regRenderable(this);
		meshData = data;
	}
	Mesh::Mesh(MeshData* data, std::string subMeshName)
	{
		Core::GetInstance()->GetRenderer()->_regRenderable(this);
		meshData = data;
	}

	Mesh::~Mesh()
	{
	}

};