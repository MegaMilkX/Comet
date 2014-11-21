#pragma once
#include "Resource.h"

#include <MeshIO.h>

namespace Comet
{

	class MeshData : public Resource
	{
	public:
		MeshData();
		~MeshData();

		void Load(std::string path);
		void Unload();

		int GetVertexAttribLayout(){ return vertexAttribLayout; }
		unsigned int GetPosBuffer(){ return bufPos; }
		unsigned int GetUVWBuffer(){ return bufUVW; }
		unsigned int GetNormBuffer(){ return bufNorm; }
		unsigned int GetColBuffer(){ return bufCol; }
		unsigned int GetFaceBuffer(){ return bufFace; }
		unsigned int GetNumFaces(){ return nFaces; }
		unsigned int GetNumVerts(){ return nVerts; }
	private:

		int vertexAttribLayout;
		unsigned int bufPos;
		unsigned int bufUVW;
		unsigned int bufNorm;
		unsigned int bufCol;
		unsigned int bufFace;

		unsigned int nVerts;
		unsigned int nFaces;
	};

};