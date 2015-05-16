#pragma once
#include "Renderable.h"

namespace Comet
{
	class QuadMeshData : public MeshData
	{
	public:
		QuadMeshData()
		{
			std::vector<float> vertices;
			vertices.push_back(0); vertices.push_back(0); vertices.push_back(0);
			vertices.push_back(1); vertices.push_back(0); vertices.push_back(0);
			vertices.push_back(0); vertices.push_back(1); vertices.push_back(0);
			vertices.push_back(1); vertices.push_back(1); vertices.push_back(0);
			FillPosition(vertices);
			FillUVW(vertices);

			std::vector<unsigned short> faces;
			faces.push_back(0); faces.push_back(1); faces.push_back(2);
			faces.push_back(1); faces.push_back(3); faces.push_back(2);
			FillIndices(faces);
			
			//SetPrimitiveType(PrimitiveType::TRISTRIP);
		}
	};

	class Sprite :
		public Renderable
	{
	public:
		Sprite();
		~Sprite();

	private:
	};

	
}