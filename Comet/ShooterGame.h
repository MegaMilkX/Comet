#pragma once

#include "BaseGame.h"
#include "Player.h"
#include "VoxelWorld.h"

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
	VoxelWorld* world;

	//temporary
	double mx, my;
	double px, py;

	Comet::Entity* camera_;
	Comet::Node* node;

	Comet::VoxelVolumeMesh* volumeMesh;
	float disp;

	Comet::Material* screenMat;
	float sTime;

	Comet::PostProcess* pp;
};

