#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "Camera.h"

#define DEGTORAD 3.1415926 / 180
#define MOVEMENTSPEED 1.8

Camera::Camera (int x, int y, int z)
{
	Position.X = x;
	Position.Y = y;
	Position.Z = z;

	Direction.X = 0;
	Direction.Y = 0;
	Direction.Z = 0;
}

Camera::~Camera(){
	
}

float Camera::calcVx()
{
	float vx = Position.X;
	vx -= 3 * sin(Direction.Y * DEGTORAD);
	return vx;
}
float Camera::calcVy()
{
	float vy = Position.Y;
	vy -= 3 * sin(Direction.X * DEGTORAD);
	return vy;
}
float Camera::calcVz()
{
	float vz = Position.Z;
	vz -= 3 * cos(Direction.Y * DEGTORAD);
	return vz;
}

void Camera::update(float dtime, int w, int a, int s, int d)
{
	if (w)
	{
		Position.Z += MOVEMENTSPEED * sin((Direction.Y - 90) * DEGTORAD) * dtime;
		Position.X -= MOVEMENTSPEED * cos((Direction.Y - 90) * DEGTORAD) * dtime;
		Position.Y += MOVEMENTSPEED * cos((Direction.X + 90) * DEGTORAD) * dtime;
	}
	if (s)
	{
		Position.Z -= MOVEMENTSPEED * sin((Direction.Y - 90) * DEGTORAD) * dtime;
		Position.X += MOVEMENTSPEED * cos((Direction.Y - 90) * DEGTORAD) * dtime;
		Position.Y -= MOVEMENTSPEED * cos((Direction.X + 90) * DEGTORAD) * dtime;
	}
	if (a)
	{
		Position.Z += MOVEMENTSPEED * sin((Direction.Y) * DEGTORAD) * dtime;
		Position.X -= MOVEMENTSPEED * cos((Direction.Y) * DEGTORAD) * dtime;
	}
	if (d)
	{
		Position.Z -= MOVEMENTSPEED * sin((Direction.Y) * DEGTORAD) * dtime;
		Position.X += MOVEMENTSPEED * cos((Direction.Y) * DEGTORAD) * dtime;
	}

	if (Position.Y < 0.3) Position.Y = 0.3;
	if (Position.Z < 0) Position.Z = 0;
	if (Position.Z > 10) Position.Z = 10;
	if (Position.X < 0) Position.X = 0;
	if (Position.X > 10) Position.X = 10;

	glutWarpPointer(200, 200);
}
