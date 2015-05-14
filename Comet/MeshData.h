#pragma once
#include "Resource.h"
#include <MeshIO.h>
#include <vector>
#include "GLBuffer.h"

#include <fbxsdk.h>
#include <fbxsdk\fileio\fbxiosettings.h>

#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <gl/glew.h>
#endif

#include "Shader.h"

namespace Comet
{
	class SubMeshData
	{
	public:
						SubMeshData(unsigned int ofst, unsigned int fcs){ ioffset = ofst; nfaces = fcs; }
						~SubMeshData(){}
		unsigned int	faces(){ return nfaces; }
		unsigned int	offset(){ return ioffset; }
		void			faces(unsigned int n){ nfaces = n; }
		void			offset(unsigned int n){ ioffset = n; }
	private:
		unsigned int	ioffset;
		unsigned int	nfaces;
	};

	class MeshData : public Resource
	{
	public:
		//Enums
		enum PrimitiveType
		{
			POINT = GL_POINT,
			LINE = GL_LINE,
			TRIANGLE = GL_TRIANGLES,
			TRISTRIP = GL_TRIANGLE_STRIP
		};

		enum BufferUsage
		{
			STATIC = GL_STATIC_DRAW,
			DYNAMIC = GL_DYNAMIC_DRAW,
			STREAM = GL_STREAM_DRAW
		};

						MeshData(BufferUsage usage = STATIC);
						~MeshData();

		void			Load(std::string path);
		void			Unload();

		virtual void	Bind(const Shader *const shader);
		virtual void	Render();
		virtual void	RenderInstanced(unsigned int count);

		PrimitiveType	GetPrimitiveType(){ return primitiveType; }
		int				GetVertexAttribLayout(){ return vertexAttribLayout; }
		unsigned int	GetPosBuffer(){ return bufPos.Id(); }
		unsigned int	GetUVWBuffer(){ return bufUVW.Id(); }
		unsigned int	GetNormBuffer(){ return bufNorm.Id(); }
		unsigned int	GetColBuffer(){ return bufCol.Id(); }
		unsigned int	GetFaceBuffer(){ return bufFace.Id(); }
		unsigned int	GetNumFaces(){ return nFaces; }
		unsigned int	GetNumVerts(){ return nVerts; }

		int				GetNumSubMeshes(){ return subMeshes.size(); }
		SubMeshData*	GetSubMesh(unsigned int id){ if (id < subMeshes.size()) return subMeshes[id]; else return 0; }

		void			FillPosition(std::vector<float> data);//three per vertex
		void			FillNormals(std::vector<float> data);//three per vertex
		void			FillColor(std::vector<float> data);//three per vertex
		void			FillIndices(std::vector<unsigned short> data);//three per face
		void			FillUVW(std::vector<float> data);		//three per vertex

		void			RebuildNormals(std::vector<float>& posdata, std::vector<unsigned short>& faces);

		void			SetPrimitiveType(PrimitiveType type){ primitiveType = type; }
	
	protected:
		void _GetMeshes(FbxNode* node, std::vector<FbxMesh*> &meshes);

		std::vector<SubMeshData*>	subMeshes;
		PrimitiveType				primitiveType;
		BufferUsage					bufferUsage;
		int							vertexAttribLayout;
		unsigned int				nVerts;
		unsigned int				nFaces;

		//----------------
		GLBuffer					bufPos;
		GLBuffer					bufUVW;
		GLBuffer					bufNorm;
		GLBuffer					bufCol;
		GLBuffer					bufFace;
	};

};