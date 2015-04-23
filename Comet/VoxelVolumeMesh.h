#pragma once

#include "Mesh.h"

namespace Comet
{

	class VoxelVolumeMesh : public Renderable
	{
	public:
		VoxelVolumeMesh();
		VoxelVolumeMesh(unsigned char w, unsigned char h = 16, unsigned char d = 16);
		~VoxelVolumeMesh();

		void _MoveNoise(float ox, float oy, float oz);
	private:
		std::vector<float> vertices;
		std::vector<float> color;
		std::vector<unsigned short> indices;
		float* voxelCloud;
		unsigned char cloudW, cloudH, cloudD;

		void _processCube(int x, int y, int z);

		float _getVoxelValue(int x, int y, int z);
	};

}