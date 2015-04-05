#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>

#define CHUNK_OBJ 0x0001
#define CHUNK_VERT 0x0002
#define CHUNK_NORM 0x0003
#define CHUNK_COL 0x0004
#define CHUNK_UVW 0x0005
#define CHUNK_FACE 0xface
#define CHUNK_BONE 0xcffa
#define CHUNK_SKIN 0xcfff

namespace MeshIO
{

enum WriteFormat
{
	MSH_BIN,
	MSH_TXT
};

struct vec3
{
	vec3()
	{
		x = 0; y = 0; z = 0;
	}
	vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float x, y, z;
};

struct SkinVert
{
	std::vector<unsigned char> bones;
	std::vector<float> weights;
};

struct Mesh
{
	std::string name;
	std::vector<vec3> verts;
	std::vector<float> vertsRAW;
	std::vector<unsigned int> faces;
	std::vector<vec3> normals;
	std::vector<vec3> color;
	std::vector<std::vector<vec3>> uvw;
	std::vector<std::vector<float>> uvwRAW;
	std::vector<std::string> bones;
	std::vector<SkinVert> skin;
};

class MeshIO
{
public:
	MeshIO(void);
	~MeshIO(void);

	void Read(std::string name);
	void Write(std::wstring name, WriteFormat fmt);

	void SwapYZ();

	void AddMesh(Mesh &msh);

	std::vector<Mesh> meshes;
private:
	
};

}