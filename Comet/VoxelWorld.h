#pragma once

#include "BaseGame.h"

#include "WorldBlock.h"

class VoxelWorld
{
public:
	VoxelWorld(Comet::Core* core);
	~VoxelWorld();

	void Update(vec3f viewPoint);
private:
	WorldBlock blocks[3 * 3 * 3];
	Comet::Material* landMaterial;
};

