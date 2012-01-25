#pragma once
#ifndef BASICS
#define BASICS

#include "CShader.h"
#include "CMeshLoader.h"

#endif
#include "Player.h"

class Camera{
public: 
	SVector3 Position, Direction;
	float velocity;	
	Player* player;

	Camera (int x, int y, int z, Player* player);
	~Camera();
	void update();
	void setLookAt();
};
