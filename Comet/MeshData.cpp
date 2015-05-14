#include "MeshData.h"

#include "Renderer.h"



namespace Comet
{

	MeshData::MeshData(BufferUsage usage)
	{
		primitiveType = TRIANGLE;
		bufferUsage = usage;
	}


	MeshData::~MeshData()
	{
		Unload();
	}
	
	void MeshData::Load(std::string path) //fbx load method
	{
		printf("Loading \"%s\": ", path.c_str());
		FbxManager* sdkManager = FbxManager::Create();
		FbxIOSettings* ios = FbxIOSettings::Create(sdkManager, IOSROOT);
		sdkManager->SetIOSettings(ios);

		FbxImporter* importer = FbxImporter::Create(sdkManager, "");

		bool importOk = importer->Initialize(path.c_str(), -1, sdkManager->GetIOSettings());

		if (!importOk)
		{
			printf("FbxImporter::Initialize() failed.\n");
			printf("Error desc: %s\n\n", importer->GetStatus().GetErrorString());
			return;
		}
		FbxScene* scene = FbxScene::Create(sdkManager, "scene");
		importer->Import(scene);
		importer->Destroy();

		//FbxAxisSystem::OpenGL.ConvertScene(scene);

		/*
		std::vector<float> vertexPool;
		std::vector<float> colorPool;
		std::vector<float> normalPool;
		std::vector<float> uvwPool;
		std::vector<unsigned short> facePool;
		for (int i = 0; i < scene->GetGeometryCount(); ++i)
		{
			FbxGeometry* geom = scene->GetGeometry(i);
			//geom->mControlPoints;
			printf("%s\n", &(geom->GetName()));

			for (int j = 0; j < geom->GetControlPointsCount(); ++j)
			{
				vertexPool.push_back(geom->mControlPoints[j].mData[0]);
			}
		}
		*/
		
		std::vector<FbxMesh*> meshes;
		FbxNode* node = scene->GetRootNode();
		_GetMeshes(node, meshes);

		std::vector<float> vertexPool;
		std::vector<float> normalPool;
		std::vector<float> uvwPool;
		std::vector<unsigned short> facePool;
		unsigned short maxIndex = 0;
		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			FbxMesh* mesh = meshes[i];
			if (!mesh->IsTriangleMesh())
				printf("Is not triangle mesh. Prepare for a mess. ");

			FbxVector4* vertices = mesh->GetControlPoints();
			FbxLayer* layer = mesh->GetLayer(0);
			FbxLayerElementNormal* normals = layer->GetNormals();
			FbxLayerElementUV* uvElement = layer->GetUVs();
			FbxLayerElementArrayTemplate<FbxVector2> uvs = uvElement->GetDirectArray();

			unsigned long nV = mesh->GetControlPointsCount();
			for (unsigned int v = 0; v < nV; ++v)
			{
				vertexPool.push_back(static_cast<float>(vertices[v].mData[0]));
				vertexPool.push_back(static_cast<float>(vertices[v].mData[2]));
				vertexPool.push_back(static_cast<float>(-vertices[v].mData[1]));

				uvwPool.push_back(static_cast<float>(uvs[v].mData[0]));
				uvwPool.push_back(static_cast<float>(uvs[v].mData[1]));
				uvwPool.push_back(static_cast<float>(uvs[v].mData[2]));
			}
			normalPool.resize(vertexPool.size());

			unsigned long polyCount = meshes[i]->GetPolygonCount();
			for (unsigned long p = 0; p < polyCount; ++p)
			{
				unsigned int vertexCount = meshes[i]->GetPolygonSize(p);

				for (unsigned int v = 0; v < vertexCount; ++v)
				{
					unsigned int vId = mesh->GetPolygonVertex(p, v);
					facePool.push_back(maxIndex + vId);
					
					FbxVector4 fbxNormal;
					mesh->GetPolygonVertexNormal(p, v, fbxNormal);
					normalPool[(maxIndex + vId) * 3] = static_cast<float>(fbxNormal.mData[0]);
					normalPool[(maxIndex + vId) * 3 + 1] = static_cast<float>(fbxNormal.mData[2]);
					normalPool[(maxIndex + vId) * 3 + 2] = static_cast<float>(-fbxNormal.mData[1]);
				}
			}
			maxIndex += static_cast<unsigned short>(nV);
		}

		FillPosition(vertexPool);
		FillNormals(normalPool);
		FillIndices(facePool);
		FillUVW(uvwPool);

		//RebuildNormals(vertexPool, facePool);

		isReady = true;
		printf("Done.\n");
	}

	void MeshData::_GetMeshes(FbxNode* node, std::vector<FbxMesh*> &meshes)
	{
		if (node->GetNodeAttribute())
		{
			if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				meshes.push_back((FbxMesh*)node->GetNodeAttribute());
			}
		}

		for (int i = 0; i < node->GetChildCount(); ++i)
		{
			_GetMeshes(node->GetChild(i), meshes);
		}
	}

	/*
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

			//TODO?
			std::vector<unsigned short> facePool;
			for (unsigned int i = 0; i < meshIO.meshes.size(); i++)
			{
				for (unsigned int j = 0; j < meshIO.meshes[i].faces.size(); j++)
				{
					meshIO.meshes[i].faces[j] += (vertexPool.size()/3);
				}
				vertexPool.insert(vertexPool.end(), meshIO.meshes[i].vertsRAW.begin(), meshIO.meshes[i].vertsRAW.end());
				subMeshes.insert(subMeshes.end(), new SubMeshData(facePool.size(), meshIO.meshes[i].faces.size() / 3));
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
			
			FillPosition(vertexPool);		
			FillUVW(uvwPool);
			FillIndices(facePool);
			///////////////////////////////////////
			RebuildNormals(vertexPool, std::vector<unsigned short>(facePool.begin(), facePool.end()));
		}

		isReady = true;
	}*/

	void MeshData::Unload()
	{
		for (unsigned int i = 0; i < subMeshes.size(); i++)
		{
			delete subMeshes[i];
		}
		subMeshes.clear();
	}

	void MeshData::Bind(const Shader *const shader)
	{
		if (vertexAttribLayout & VATTR_POS)
		{
			glEnableVertexAttribArray(shader->GetAttribLocation(Shader::POS));	//координаты
			glBindBuffer(GL_ARRAY_BUFFER, bufPos.Id());
			glVertexAttribPointer(shader->GetAttribLocation(Shader::POS), 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (vertexAttribLayout & VATTR_UVW)
		{
			glEnableVertexAttribArray(shader->GetAttribLocation(Shader::UVW));	//uvw
			glBindBuffer(GL_ARRAY_BUFFER, bufUVW.Id());
			glVertexAttribPointer(shader->GetAttribLocation(Shader::UVW), 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (vertexAttribLayout & VATTR_NOR)
		{
			glEnableVertexAttribArray(shader->GetAttribLocation(Shader::NRM));	//нормали
			glBindBuffer(GL_ARRAY_BUFFER, bufNorm.Id());
			glVertexAttribPointer(shader->GetAttribLocation(Shader::NRM), 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (vertexAttribLayout & VATTR_COL)
		{
			glEnableVertexAttribArray(shader->GetAttribLocation(Shader::COL));	//цвет
			glBindBuffer(GL_ARRAY_BUFFER, bufCol.Id());
			glVertexAttribPointer(shader->GetAttribLocation(Shader::COL), 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
	}

	void MeshData::Render()
	{
		if (bufFace.Id())
		{
			if (subMeshes.size() > 0)
			{
				for (unsigned int i = 0; i < subMeshes.size(); i++)
				{
					SubMeshData* subMesh = subMeshes[i];
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufFace.Id());
					glDrawElements(primitiveType, subMesh->faces() * 3, GL_UNSIGNED_SHORT, (void*)(subMesh->offset()));
				}
			}
			else
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufFace.Id());
				glDrawElements(primitiveType, nFaces * 3, GL_UNSIGNED_SHORT, 0);
			}
		}
		else // No indices - no submeshes. TODO At least for now
			glDrawArrays(primitiveType, 0, nVerts);
	}

	void MeshData::RenderInstanced(unsigned int count)
	{
		if (bufFace.Id())
		{
			if (subMeshes.size() > 0)
			{
				for (unsigned int i = 0; i < subMeshes.size(); i++)
				{
					SubMeshData* subMesh = subMeshes[i];
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufFace.Id());
					glDrawElementsInstanced(primitiveType, subMesh->faces() * 3, GL_UNSIGNED_SHORT, (void*)(subMesh->offset()), count);
				}
			}
			else
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufFace.Id());
				glDrawElementsInstanced(primitiveType, nFaces * 3, GL_UNSIGNED_SHORT, 0, count);
			}
		}
		else // No indices - no submeshes. TODO At least for now
			glDrawArraysInstanced(primitiveType, 0, nVerts, count);
	}

	void MeshData::FillPosition(std::vector<float> data)
	{
		vertexAttribLayout |= VATTR_POS;
		bufPos.Fill(data);
		nVerts = data.size()/3;
	}
	void MeshData::FillNormals(std::vector<float> data)
	{
		vertexAttribLayout |= VATTR_NOR;
		bufNorm.Fill(data);
	}
	void MeshData::FillColor(std::vector<float> data)
	{
		vertexAttribLayout |= VATTR_COL;
		bufCol.Fill(data);
	}
	void MeshData::FillIndices(std::vector<unsigned short> data)
	{
		bufFace.Fill(data);
		nFaces = data.size()/3;
	}
	void MeshData::FillUVW(std::vector<float> data)
	{
		vertexAttribLayout |= VATTR_UVW;
		bufUVW.Fill(data);
	}

	void MeshData::RebuildNormals(std::vector<float>& posdata, std::vector<unsigned short>& faces)
	{
		//Reserve an array for normals
		std::vector<float> normals(nVerts * 3, 0);
		//normals.resize(nVerts * 3);
			
		//Calculare normals using indices
		vec3f a(0, 0, 0), b(0, 0, 0), c(0, 0, 0);
		for (unsigned int i = 0; i < nFaces * 3; i += 3)
		{
			//faces[i];//v0
			//faces[i+1];//v1
			//faces[i+2];//v2
				
			//Fill a, b and c
			a = vec3f(posdata[faces[i] * 3], posdata[faces[i] * 3 + 1], posdata[faces[i] * 3 + 2]);
			b = vec3f(posdata[faces[i + 1] * 3], posdata[faces[i + 1] * 3 + 1], posdata[faces[i + 1] * 3 + 2]);
			c = vec3f(posdata[faces[i + 2] * 3], posdata[faces[i + 2] * 3 + 1], posdata[faces[i + 2] * 3 + 2]);

			b = b - a;
			c = c - a;
			a = cross(b, c);

			normals[faces[i] * 3] += a.x;
			normals[faces[i] * 3 + 1] += a.y;
			normals[faces[i] * 3 + 2] += a.z;

			normals[faces[i + 1] * 3] += a.x;
			normals[faces[i + 1] * 3 + 1] += a.y;
			normals[faces[i + 1] * 3 + 2] += a.z;

			normals[faces[i + 2] * 3] += a.x;
			normals[faces[i + 2] * 3 + 1] += a.y;
			normals[faces[i + 2] * 3 + 2] += a.z;
		}

		for (unsigned int i = 0; i < normals.size(); i += 3)
		{
			vec3f norm(normals[i], normals[i + 1], normals[i + 2]);
			float len = norm.length();
			normals[i] = norm.x / len;
			normals[i + 1] = norm.y / len;
			normals[i + 2] = norm.z / len;
		}

		FillNormals(normals);
	}

};