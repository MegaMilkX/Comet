#include "MeshData.h"

#include "Renderer.h"

namespace Comet
{

	MeshData::MeshData()
	{
		bufPos = 0;
		bufUVW = 0;
		bufNorm = 0;
		bufCol = 0;
		bufFace = 0;
		primitiveType = TRIANGLE;
	}


	MeshData::~MeshData()
	{
		Unload();
	}

	void MeshData::Load(std::string path)
	{
		MeshIO::MeshIO meshIO;
		meshIO.Read(path);

		if (meshIO.meshes.size() > 0)
		{
			//Записать все меши в один буфер
			std::vector<float> vertexPool;
			std::vector<float> colorPool;
			std::vector<float> normalPool;
			std::vector<float> uvwPool;
			vertexPool.clear();
			uvwPool.clear();

			std::vector<unsigned int> facePool;
			for (unsigned int i = 0; i < meshIO.meshes.size(); i++)
			{
				for (unsigned int j = 0; j < meshIO.meshes[i].faces.size(); j++)
				{
					meshIO.meshes[i].faces[j] += (vertexPool.size()/3);
				}
				vertexPool.insert(vertexPool.end(), meshIO.meshes[i].vertsRAW.begin(), meshIO.meshes[i].vertsRAW.end());
				facePool.insert(facePool.end(), meshIO.meshes[i].faces.begin(), meshIO.meshes[i].faces.end());

				//Доделать для нескольких слоев текстур																				!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				if (meshIO.meshes[i].uvwRAW.size()>0)
				{
					uvwPool.insert(uvwPool.end(), meshIO.meshes[i].uvwRAW[0].begin(), meshIO.meshes[i].uvwRAW[0].end());
				}
			}

			nFaces = facePool.size() / 3;
			nVerts = vertexPool.size() / 3;

			vertexAttribLayout = 0;
			//TODO
			vertexAttribLayout = VATTR_POS | VATTR_UVW;

			glGenBuffers(1, &bufPos);
			glBindBuffer(GL_ARRAY_BUFFER, bufPos);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (vertexPool.size()), &vertexPool[0], GL_STATIC_DRAW);

			float col_data[] = {
				0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 0.0f
			};

			glGenBuffers(1, &bufCol);
			glBindBuffer(GL_ARRAY_BUFFER, bufCol);
			glBufferData(GL_ARRAY_BUFFER, sizeof(col_data), col_data, GL_STATIC_DRAW);

			unsigned short *face_data = new unsigned short[facePool.size()];

			for (int i = 0; i < facePool.size(); i++)
			{
				face_data[i] = facePool[i];
			}
			
			if (uvwPool.size()>0)
			{
				glGenBuffers(1, &bufUVW);
				glBindBuffer(GL_ARRAY_BUFFER, bufUVW);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)* (uvwPool.size()), &uvwPool[0], GL_STATIC_DRAW);
			}

			glGenBuffers(1, &bufFace);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufFace);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*(facePool.size()), face_data, GL_STATIC_DRAW);
			delete face_data;
		}
		else
		{
			vertexAttribLayout = VATTR_POS | VATTR_COL;

			float pos_data[] = {
				0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 0.0f
			};

			glGenBuffers(1, &bufPos);
			glBindBuffer(GL_ARRAY_BUFFER, bufPos);
			glBufferData(GL_ARRAY_BUFFER, sizeof(pos_data), pos_data, GL_STATIC_DRAW);

			float col_data[] = {
				0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 0.0f
			};

			glGenBuffers(1, &bufCol);
			glBindBuffer(GL_ARRAY_BUFFER, bufCol);
			glBufferData(GL_ARRAY_BUFFER, sizeof(col_data), col_data, GL_STATIC_DRAW);

			unsigned short face_data[] = {
				0, 1, 2,
				0, 1, 3,
				0, 2, 3,
				1, 3, 4,
				2, 5, 3,
				2, 1, 6
			};

			glGenBuffers(1, &bufFace);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufFace);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face_data), face_data, GL_STATIC_DRAW);


			nFaces = 6;
			nVerts = 7;
		}

		isReady = true;
	}

	void MeshData::Unload()
	{
		glDeleteBuffers(1, &bufPos);
		glDeleteBuffers(1, &bufCol);
		glDeleteBuffers(1, &bufFace);
		glDeleteBuffers(1, &bufUVW);
		glDeleteBuffers(1, &bufNorm);
	}

	void MeshData::FillPosition(std::vector<float> data)
	{
		glDeleteBuffers(1, &bufPos);

		vertexAttribLayout |= VATTR_POS;
		glGenBuffers(1, &bufPos);
		glBindBuffer(GL_ARRAY_BUFFER, bufPos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* (data.size()), &data[0], GL_STATIC_DRAW);

		nVerts = data.size()/3;
	}
	void MeshData::FillNormals(std::vector<float> data)
	{
		glDeleteBuffers(1, &bufNorm);

		vertexAttribLayout |= VATTR_NOR;
	}
	void MeshData::FillColor(std::vector<float> data)
	{
		glDeleteBuffers(1, &bufCol);

		vertexAttribLayout |= VATTR_COL;
	}
	void MeshData::FillIndices(std::vector<unsigned short> data)
	{
		glDeleteBuffers(1, &bufFace);

		glGenBuffers(1, &bufFace);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufFace);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*(data.size()), &data[0], GL_STATIC_DRAW);

		nFaces = data.size()/3;
	}
	void MeshData::FillUVW(std::vector<float> data)
	{
		glDeleteBuffers(1, &bufUVW);

		vertexAttribLayout |= VATTR_UVW;
		glGenBuffers(1, &bufUVW);
		glBindBuffer(GL_ARRAY_BUFFER, bufUVW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* (data.size()), &data[0], GL_STATIC_DRAW);
	}

};