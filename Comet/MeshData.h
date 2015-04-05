#pragma once
#include "Resource.h"
#include <MeshIO.h>
#include <vector>

namespace Comet
{
	class SubMeshData
	{
	public:
		SubMeshData();
		~SubMeshData();
		unsigned int faces(){ return nfaces; }
	private:

		unsigned int nfaces;
	};

	class MeshData : public Resource
	{
	public:
		MeshData();
		~MeshData();

		enum PrimitiveType
		{
			POINT,
			LINE,
			TRIANGLE,
			TRISTRIP
		};

		void Load(std::string path);
		void Unload();

		PrimitiveType GetPrimitiveType(){ return primitiveType; }
		int GetVertexAttribLayout(){ return vertexAttribLayout; }
		unsigned int GetPosBuffer(){ return bufPos; }
		unsigned int GetUVWBuffer(){ return bufUVW; }
		unsigned int GetNormBuffer(){ return bufNorm; }
		unsigned int GetColBuffer(){ return bufCol; }
		unsigned int GetFaceBuffer(){ return bufFace; }
		unsigned int GetNumFaces(){ return nFaces; }
		unsigned int GetNumVerts(){ return nVerts; }

		int GetNumSubMeshes(){ return subMeshes.size(); }
		SubMeshData* GetSubMesh(unsigned int id){ if (id < subMeshes.size()) return subMeshes[id]; else return 0; }

		void FillPosition(std::vector<float> data);//three per vertex
		void FillNormals(std::vector<float> data);//three per vertex
		void FillColor(std::vector<float> data);//three per vertex
		void FillIndices(std::vector<unsigned short> data);//three per face
		void FillUVW(std::vector<float> data);		//three per vertex

		void SetPrimitiveType(PrimitiveType type){ primitiveType = type; }
	private:
		std::vector<SubMeshData*> subMeshes;
		PrimitiveType primitiveType;
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