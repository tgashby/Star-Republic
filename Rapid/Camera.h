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

	Camera (int x, int y, int z, Player* player, Map const *map );
	~Camera();
	void update();
	void setLookAt();
   SVector3 getPosition();
private:
	Player* player;
   Map const *m_map;
};
