#pragma once
#include "Mesh.h"

#include <math3f.h>

namespace Comet
{

	class SkinMesh :
		public Mesh
	{
	public:
		SkinMesh();
		~SkinMesh();

		virtual void	Render(const Camera *const cam);

		void SetNode(Node* e);
	private:
		void _CollectBones(Node* e);

		std::vector<mat4f> bindPoseTransforms;
	};

}