#include "VoxelWorld.h"

using namespace Comet;

VoxelWorld::VoxelWorld(Core* core)
{
	landMaterial = new Material("data\\shaders\\deferredmarchingcubes.glsl", "data\\textures\\GrassDead0102_1_S.jpg");
	landMaterial->SetTexture2D("data\\textures\\Grass0026_1_S.jpg", 1);

	for (int z = 0; z < 3; z++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				//TODO create world block
			}
		}
	}
}


VoxelWorld::~VoxelWorld()
{
	for (int z = 0; z < 3; z++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				//TODO delete crap
			}
		}
	}

	delete landMaterial;
}

void VoxelWorld::Update(glm::vec3 viewPoint)
{
	glm::ivec3 centerBlock = glm::ivec3(viewPoint.x / 16, viewPoint.y / 16, viewPoint.z / 16);
}