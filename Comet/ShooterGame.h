#pragma once

#include "BaseGame.h"
#include "VoxelVolumeMesh.h"

class ShooterGame : public Comet::BaseGame
{
public:
	ShooterGame();
	~ShooterGame();

	void Init();
	void PostInit();
	void Start();
	bool Update();
private:
	double mx, my;
	double px, py;

	Comet::Entity* camera_;

	Comet::VoxelVolumeMesh* volumeMesh;
	float disp;

	Comet::Material* screenMat;
	float sTime;
};

