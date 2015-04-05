#include "MeshIO.h"

#include <Windows.h>

namespace MeshIO
{

MeshIO::MeshIO(void)
{
}

MeshIO::~MeshIO(void)
{
}

void MeshIO::Read(std::string name)
{
	meshes.clear();

	unsigned int chunk;
	unsigned int sz;
	unsigned int subsz;

	FILE* file;
	file = fopen(name.c_str(), "rb");

	if (!file)
	{
		printf("[ER] \"%s\" not found", name.c_str());
		return;
	}

	Mesh mesh; //
	int currentMesh = -1;
	int currentUVW = 0;
	std::vector<float> uvwset;

	float temp;

	do
	{
		if (1 > fread((void*)&chunk, 4, 1, file))
			chunk = 0;
		fread((void*)&sz, 4, 1, file);

		switch (chunk)
		{
		case CHUNK_OBJ:
			mesh.name.resize(sz);
			fread((void*)&(mesh.name)[0], sizeof(char), sz, file);
			mesh.verts.clear();
			mesh.color.clear();
			mesh.faces.clear();
			mesh.normals.clear();
			mesh.skin.clear();
			mesh.uvw.clear();
			mesh.bones.clear();
			meshes.push_back(mesh);
			currentMesh++;
			currentUVW = 0;

			printf(mesh.name.c_str());
			printf("\n");
			break;
		case CHUNK_VERT:
			printf("CHUNK_VERT reading... \n");
			meshes.at(currentMesh).vertsRAW.resize(sz * 3);
			//meshes.at(currentMesh).verts.resize(sz);
			for (int i = 0; i < sz*3; i+=3)
			{
				fread((void*)&(meshes.at(currentMesh).vertsRAW.at(i)), 4, 1, file);
				fread((void*)&(meshes.at(currentMesh).vertsRAW.at(i+1)), 4, 1, file);
				fread((void*)&(meshes.at(currentMesh).vertsRAW.at(i+2)), 4, 1, file);
			}
			printf("CHUNK_VERT read complete \n");
			break;
		case CHUNK_FACE:
			printf("CHUNK_FACE reading... \n");
			meshes.at(currentMesh).faces.resize(sz);
			for (int i = 0; i < sz; i++)
			{
				fread((void*)&(meshes.at(currentMesh).faces.at(i)), 4, 1, file);
			}
			printf("CHUNK_FACE read complete \n");
			break;
		case CHUNK_NORM:
			printf("CHUNK_NORM reading... \n");
			meshes.at(currentMesh).normals.resize(sz);
			for (int i = 0; i < sz; i++)
			{
				fread((void*)&(meshes.at(currentMesh).normals.at(i).x), 4, 1, file);
				fread((void*)&(meshes.at(currentMesh).normals.at(i).y), 4, 1, file);
				fread((void*)&(meshes.at(currentMesh).normals.at(i).z), 4, 1, file);
			}
			printf("CHUNK_NORM read complete \n");
			break;
		case CHUNK_COL:
			printf("CHUNK_COL reading... \n");
			meshes.at(currentMesh).color.resize(sz);
			for (int i = 0; i < sz; i++)
			{
				fread((void*)&(meshes.at(currentMesh).color.at(i).x), 4, 1, file);
				fread((void*)&(meshes.at(currentMesh).color.at(i).y), 4, 1, file);
				fread((void*)&(meshes.at(currentMesh).color.at(i).z), 4, 1, file);
			}
			printf("CHUNK_COL read complete \n");
			break;
		case CHUNK_UVW:
			printf("CHUNK_UVW reading... \n");
			uvwset.clear(); 
			meshes.at(currentMesh).uvwRAW.push_back(uvwset);
			meshes.at(currentMesh).uvwRAW[currentUVW].resize(sz*3);
			for (int i = 0; i < sz*3; i+=3)
			{
				fread((void*)&(meshes.at(currentMesh).uvwRAW[currentUVW].at(i)), 4, 1, file);
				fread((void*)&(meshes.at(currentMesh).uvwRAW[currentUVW].at(i + 1)), 4, 1, file);
				fread((void*)&(meshes.at(currentMesh).uvwRAW[currentUVW].at(i + 2)), 4, 1, file);
			}
			currentUVW++;
			printf("CHUNK_UVW read complete \n");
			break;
		case CHUNK_BONE:
			printf("CHUNK_BONE reading... \n");
			meshes.at(currentMesh).bones.resize(sz);
			for (int i = 0; i < sz; i++)
			{
				fread((void*)&subsz, 4, 1, file);
				meshes.at(currentMesh).bones[i].resize(subsz);
				fread((void*)&(meshes.at(currentMesh).bones[i])[0], sizeof(char), subsz, file);
			}
			printf("CHUNK_BONE read complete \n");
			break;
		case CHUNK_SKIN:
			printf("CHUNK_SKIN reading... \n");
			meshes.at(currentMesh).skin.resize(sz);
			for (int i = 0; i < sz; i++)
			{
				fread((void*)&subsz, 1, 1, file);
				meshes.at(currentMesh).skin[i].bones.resize(subsz);
				meshes.at(currentMesh).skin[i].weights.resize(subsz);
				for (int j = 0; j < subsz; j++)
				{
					fread((void*)&(meshes.at(currentMesh).skin[i].bones[j]), 1, 1, file);
					fread((void*)&(meshes.at(currentMesh).skin[i].weights[j]), 4, 1, file);
				}
			}
			printf("CHUNK_SKIN read complete \n");
			break;
		}
	} while (chunk != 0);

	fclose(file);
}

void MeshIO::Write(std::wstring name, WriteFormat fmt)
{
	std::ofstream osfile(name.c_str(), std::ios::out | std::ios::binary);

	unsigned int chunk;
	unsigned int sz;

	for(unsigned int i = 0; i < meshes.size(); i++)
	{
		chunk = CHUNK_OBJ;						//Объект
		osfile.write((char*)&chunk, 4);
		sz = meshes[i].name.size();
		osfile.write((char*)&sz, 4);
		osfile.write((char*)(meshes[i].name.c_str()), meshes[i].name.size());

		if (meshes[i].verts.size()>0)			//Вершины
		{
			chunk = CHUNK_VERT;
			osfile.write((char*)&chunk, 4);
			sz = meshes[i].verts.size();
			osfile.write((char*)&sz, 4);
			for (unsigned int j = 0; j < meshes[i].verts.size(); j++)
			{
				osfile.write((char*)&(meshes[i].verts[j].x), 4);
				osfile.write((char*)&(meshes[i].verts[j].y), 4);
				osfile.write((char*)&(meshes[i].verts[j].z), 4);
			}
		}

		if (meshes[i].faces.size()>0)			//Индексы
		{
			chunk = CHUNK_FACE;
			osfile.write((char*)&chunk, 4);
			sz = meshes[i].faces.size();		//Faces.size - количество индексов, а не треугольников. Треугольники = faces.size()/3
			osfile.write((char*)&sz, 4);
			for (unsigned int j = 0; j < meshes[i].faces.size(); j++)
			{
				osfile.write((char*)&(meshes[i].faces[j]), 4);
			}
		}

		if (meshes[i].normals.size()>0)			//Нормали
		{
			chunk = CHUNK_NORM;
			osfile.write((char*)&chunk, 4);
			sz = meshes[i].normals.size();
			osfile.write((char*)&sz, 4);
			for (unsigned int j = 0; j < meshes[i].normals.size(); j++)
			{
				osfile.write((char*)&(meshes[i].normals[j].x), 4);
				osfile.write((char*)&(meshes[i].normals[j].y), 4);
				osfile.write((char*)&(meshes[i].normals[j].z), 4);
			}
		}

		if (meshes[i].color.size()>0)			//Цвет
		{
			chunk = CHUNK_COL;
			osfile.write((char*)&chunk, 4);
			sz = meshes[i].color.size();
			osfile.write((char*)&sz, 4);
			for (unsigned int j = 0; j < meshes[i].color.size(); j++)
			{
				osfile.write((char*)&(meshes[i].color[j].x), 4);
				osfile.write((char*)&(meshes[i].color[j].y), 4);
				osfile.write((char*)&(meshes[i].color[j].z), 4);
			}
		}

		if (meshes[i].uvw.size()>0)				//Текстурные координаты
		{
			for (unsigned int j = 0; j < meshes[i].uvw.size(); j++)
			{
				if (meshes[i].uvw[j].size()>0)
				{
					chunk = CHUNK_UVW;
					osfile.write((char*)&chunk, 4);
					sz = meshes[i].uvw[j].size();
					osfile.write((char*)&sz, 4);
					for (unsigned int k = 0; k < meshes[i].uvw[j].size(); k++)
					{
						osfile.write((char*)&(meshes[i].uvw[j].at(k).x), 4);
						osfile.write((char*)&(meshes[i].uvw[j].at(k).y), 4);
						osfile.write((char*)&(meshes[i].uvw[j].at(k).z), 4);
					}
				}
			}
		}

		if (meshes[i].bones.size()>0)			//Список костей
		{
			chunk = CHUNK_BONE;
			osfile.write((char*)&chunk, 4);
			sz = meshes[i].bones.size();
			osfile.write((char*)&sz, 4);

			for (unsigned int j = 0; j < meshes[i].bones.size(); j++)
			{
				sz = meshes[i].bones[j].size();
				osfile.write((char*)&sz, 4);
				osfile.write(meshes[i].bones[j].c_str(), meshes[i].bones[j].size());
			}
		}

		if (meshes[i].skin.size()>0)			//Bone weights
		{
			chunk = CHUNK_SKIN;
			osfile.write((char*)&chunk, 4);
			sz = meshes[i].skin.size();
			osfile.write((char*)&sz, 4);
			for (unsigned int j = 0; j < meshes[i].skin.size(); j++) //количество вертексов
			{
				unsigned char c_sz = meshes[i].skin[j].bones.size();
				osfile.write((char*)&c_sz, 1);					//количество костей на этот вертекс

				for (unsigned char k = 0; k < c_sz; k++)
				{
					c_sz = meshes[i].skin[j].bones[k];
					osfile.write((char*)&c_sz, 1);				//ID кости? 
					float wght = meshes[i].skin[j].weights[k];
					osfile.write((char*)&wght, 4);				//вес
				}
			}
		}
	}

	osfile.close();
}

void MeshIO::SwapYZ()
{
	float a;
	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j < meshes[i].verts.size(); j++)
		{
			a = meshes[i].verts[j].y;
			meshes[i].verts[j].y = meshes[i].verts[j].z;
			meshes[i].verts[j].z = -a;
		}
		for (int j = 0; j < meshes[i].normals.size(); j++)
		{
			a = meshes[i].normals[j].y;
			meshes[i].normals[j].y = meshes[i].normals[j].z;
			meshes[i].normals[j].z = -a;
		}
		for (int j = 0; j < meshes[i].vertsRAW.size(); j+=3)
		{
			a = meshes[i].vertsRAW[j+1];
			meshes[i].vertsRAW[j + 1] = meshes[i].vertsRAW[j + 2];
			meshes[i].vertsRAW[j + 2] = -a;
		}

		for (int j = 0; j < meshes[i].faces.size(); j += 3)
		{
			
		}
	}
}

void MeshIO::AddMesh(Mesh &msh)
{
	meshes.insert(meshes.end(), msh);
}

};