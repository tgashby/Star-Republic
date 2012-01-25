#include <iostream>

#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#include <stdio.h>


#include "HUD.h"



HUD::HUD()
{ }

HUD::~HUD()
{ }

void HUD::renderBitmapString (float x, float y, float z, char *string) 
{  
  char *c;
  glRasterPos3f(x, y, z);
  for (c=string; *c != '\0'; c++) 
  {
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
  }
}

void HUD::drawText(int FPS, int curTime)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 	gluLookAt(0.0, 0.0, 1.0, 
            0.0, 0.0, 0.0, 
            0.0, 1.0, 0.0);
	GLfloat color[3] = {0.4, 1.0, 0.1};
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color); 
		
 	GLfloat lightDir[] = {0, 0, -1, 0.0};
	GLfloat diffuseComp[] = {1.0, 1.0, 1.0, 1.0};

	glEnable(GL_LIGHT0);
 	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
 	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseComp);

	char timer[14];
	timer[0] = (((int)curTime)/10000) % 10 + 48;
	timer[1] = (((int)curTime)/1000) % 10 + 48;
	timer[2] ='s';
	timer[3]='\0';
	renderBitmapString(-0.25, 2.7, -4.5, timer);

	char fps[9] = "FPS: ";
	fps[5] = FPS/100 % 10 + 48;
	fps[6] = FPS/10 % 10 + 48;
	fps[7] = FPS/1 % 10 + 48;
	renderBitmapString(-0.6, 2.5, -4.5, fps);

	glDisable(GL_LIGHT0);
}
