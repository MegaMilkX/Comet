#include "MarchingStarMesh.h"

int main()
{
	MarchingStarMesh* mesh = new MarchingStarMesh();
	
	mesh->Build();


	delete mesh;
	return 0;
}