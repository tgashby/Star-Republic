#ifndef BASICS
#define BASICS

#include "CShader.h"
#include "CMeshLoader.h"

#endif

class Camera{
public: 
	SVector3 Position, Direction;
	float velocity;	

	Camera (int x, int y, int z);
	~Camera();
	float calcVx();
	float calcVy();
	float calcVz();
	void update(float dtime, int w, int a, int s, int d);
};
