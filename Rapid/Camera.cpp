
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "Camera.h"

#define DEGTORAD 3.1415926 / 180
#define MOVEMENTSPEED 1.8

Camera::Camera (int x, int y, int z, Player* p)
{
	Position.X = x;
	Position.Y = y;
	Position.Z = z;

	Direction.X = 0;
	Direction.Y = 0;
	Direction.Z = 1;

	player = p;
}

Camera::~Camera(){
	
}


void Camera::update()
{
   Position.Z = player->getPosition()->Z - 3;
}

void Camera::setLookAt()
{
   gluLookAt(
      Position.X, Position.Y, Position.Z, 
      Position.X, Position.Y, Position.Z + Direction.Z, 
      0, 1, 0);
}

SVector3 Camera::getPosition()
{
   return Position;
}
