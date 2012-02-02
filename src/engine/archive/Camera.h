#pragma once

#include "ProjIncludes.h"

class Camera
{
public:
	Camera();
	~Camera();

	void moveOver(GLfloat xVal, GLfloat yVal);
	void setPosition(GLfloat xPos, GLfloat yPos);

	GLfloat getX();
	GLfloat	getY();

private:
	GLfloat xPos, yPos;
};


