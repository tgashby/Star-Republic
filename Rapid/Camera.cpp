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
	Direction.Z = 1;
}

Camera::~Camera(){
	
}


void Camera::update()
{
	
}

void Camera::setLookAt()
{
   gluLookAt(
      Position.X, Position.Y, Position.Z, 
      Position.X, Position.Y, Position.Z + Direction.Z, 
      0, 1, 0);
}
