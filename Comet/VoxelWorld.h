#pragma once

#include "BaseGame.h"

#include "WorldBlock.h"

class VoxelWorld
{
public:
	VoxelWorld(Comet::Core* core);
	~VoxelWorld();

	void Update(glm::vec3 viewPoint);
private:
	WorldBlock blocks[3 * 3 * 3];
	Comet::Material* landMaterial;
};

