#pragma once
#include "Mesh.h"

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
	};

}