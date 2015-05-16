#include "SkinMesh.h"

namespace Comet
{

	SkinMesh::SkinMesh()
	{
	}


	SkinMesh::~SkinMesh()
	{
	}

	void	SkinMesh::Render(const Camera *const cam)
	{
		//TODO: Pass bone transforms to the shader here

		Mesh::Render(cam);
	}

	void	SkinMesh::SetNode(Node* e)
	{
		Component::SetNode(e);
		_CollectBones(e);
	}

	void	SkinMesh::_CollectBones(Node* e)
	{
		std::cout << e->GetName() << std::endl;

		bindPoseTransforms.clear();
		bindPoseTransforms.push_back(e->GetLocalTransform());

		for (unsigned int i = 0; i < e->GetChildCount(); ++i)
			_CollectBones(e->GetChild(i));
	}
}