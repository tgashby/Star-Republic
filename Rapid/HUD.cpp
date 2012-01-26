#include <iostream>

#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#ifdef _WIN32
#include <GL\glew.h>
#include <GL\glut.h>
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

void HUD::drawText(int FPS, int curTime, int playerHealth)
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
	timer[0] = (((int)curTime)/100000) % 10 + 48;
	timer[1] = (((int)curTime)/10000) % 10 + 48;
	timer[2] = (((int)curTime)/1000) % 10 + 48;
	timer[3] ='s';
	timer[4]='\0';
	renderBitmapString(-0.2, 2.7, -4.5, timer);

	char fps[9] = "FPS: ";
	fps[5] = FPS/100 % 10 + 48;
	fps[6] = FPS/10 % 10 + 48;
	fps[7] = FPS/1 % 10 + 48;
	renderBitmapString(-0.45, 2.5, -4.5, fps);
        
        char ph[9] = "FPS: ";
	ph[0] = playerHealth/100 % 10 + 48;
	ph[1] = playerHealth/10 % 10 + 48;
	ph[2] = playerHealth/1 % 10 + 48;
	ph[3] = ' ';
        ph[4] = '%';
        ph[5] = '\0';
	renderBitmapString(-0.23, 2.3, -4.5, ph);

	glDisable(GL_LIGHT0);
}

void HUD::drawWin()
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

        char win[] = "Congratulations!";
	renderBitmapString(-0.5, 0.7, -4.5, win);

	glDisable(GL_LIGHT0);
}
void HUD::drawLose()
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

        char win[] = "Mission Failed!";
	renderBitmapString(-0.5, 0.7, -4.5, win);

	glDisable(GL_LIGHT0);
}

void HUD::renderGlutAimer(float px, float py, float dx, float dy)
{
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0, 0.0, 1.0, 
            0.0, 0.0, 0.0, 
            0.0, 1.0, 0.0);
   GLfloat color[3] = {0.4, 1.0, 0.1};
	
   glMaterialfv(GL_FRONT, GL_DIFFUSE, color); 
		
   GLfloat lightDir[] = {0, -1, -1, 0.0};
   GLfloat diffuseComp[] = {0.1, 0.1, 0.1, 1.0};

   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseComp);

   glTranslatef(dx, dy, -9);
   glutSolidTorus(0.02, 0.6, 8, 8);
   glTranslatef(-dx, -dy, 0);
   dx = 0.4 * px + 0.6 * dx;
   dy = 0.4 * py + 0.6 * dy;

   glTranslatef(dx, dy, 2);
   glutSolidTorus(0.02, 0.7, 8, 8);
   glTranslatef(-dx, -dy, 0);
   dx = 0.5 * px + 0.5 * dx;
   dy = 0.5 * py + 0.5 * dy;

   glTranslatef(dx, dy, 2);
   glutSolidTorus(0.02, 0.8, 8, 8);
   glTranslatef(-dx, -dy, 0);

   glDisable(GL_LIGHT0);
}


