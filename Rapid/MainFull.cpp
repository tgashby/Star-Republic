#include <iostream>

#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#include <stdio.h>


// Utility classes for loading shaders/meshes
#include "CShader.h"
#include "CMeshLoader.h"
#include "Enemy.h"
#include "Camera.h"
#include "Map.h"

#define LOOK_SPEED 0.1


/********************
 * Global Varaibles *
 ********************/


// Window information
int WindowWidth = 400, WindowHeight = 400;

// Time-independant movement variables
int Time0, Time1;

int prevX, prevY;
int w = 0, a = 0, s = 0, d = 0;

int curTime = 0;
int numFrames = 0, lastSecond = 0, FPS = 0;
time_t *timeTracker;
int numEnemiesKilled = 0, numEnemiesOnScreen = 1;

Enemy *e;
Camera *camera;
Map *map;

/***************************
 * Drawing Text            *
 ***************************/

void renderBitmapString (float x, float y, float z, char *string) 
{  
  char *c;
  glRasterPos3f(x, y, z);
  for (c=string; *c != '\0'; c++) 
  {
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
  }
}

void drawText()
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

	if (curTime > 59000)
	{
		char* gameover = "Game Over";
		renderBitmapString(-0.3, -0.3, -4.5, gameover);
	}

	char timer[14];
	timer[0] = (((int)curTime)/10000) % 10 + 48;
	timer[1] = (((int)curTime)/1000) % 10 + 48;
	timer[2] =' ';
	timer[3] ='/';
	timer[4] =' ';
	timer[5] ='6';
	timer[6] ='0';
	timer[7]='\0';
	renderBitmapString(-2.7, 2.7, -4.5, timer);


	char screen[15] = "On Screen: ";
	screen[11] = numEnemiesOnScreen/100 % 10 + 48;
	screen[12] = numEnemiesOnScreen/10 % 10 + 48;
	screen[13] = numEnemiesOnScreen/1 % 10 + 48;
	renderBitmapString(1.1, 2.7, -4.5, screen);

	char caught[12] = "Caught: ";
	caught[8] = numEnemiesKilled/100 % 10 + 48;
	caught[9] = numEnemiesKilled/10 % 10 + 48;
	caught[10] = numEnemiesKilled/1 % 10 + 48;
	renderBitmapString(1.53, 2.5, -4.5, caught);

	char fps[9] = "FPS: ";
	fps[5] = FPS/100 % 10 + 48;
	fps[6] = FPS/10 % 10 + 48;
	fps[7] = FPS/1 % 10 + 48;
	renderBitmapString(-2.7, 2.5, -4.5, fps);

	glDisable(GL_LIGHT0);
}

/***************************
 * Enemies and Game Logic  *
 ***************************/

bool checkProximity(Enemy *first, Enemy *second)
{
	if (first->Translation.X - first->size < second->Translation.X + second->size && first->Translation.X + first->size > second->Translation.X - second->size)
	{
		if (first->Translation.Z - first->size < second->Translation.Z + second->size && first->Translation.Z + first->size > second->Translation.Z - second->size)
		{
			return true;
		}
	}

	return false;
}

void addEnemy()
{
	Enemy *temp = new Enemy(rand());
	Enemy *tocheck = e;
	while (tocheck != 0)
	{
		if (checkProximity(temp, tocheck))
		{
			temp = new Enemy(rand());
			tocheck = e;
		}
		else
			tocheck = tocheck->next;
	}

	temp->next = e;
	e = temp;
	numEnemiesOnScreen++;
}

int shouldAddEnemy()
{
	if ((numEnemiesOnScreen + numEnemiesKilled) * 3 < curTime / 1000)
		return 1;
	return 0;
}

void checkNumEnemies()
{
	int num = 0;

	Enemy *temp = e;
    	while(temp != 0)
	{
		if (temp->alive)
			num++;
		temp = temp->next;
	}

	if (numEnemiesOnScreen != num)
		numEnemiesOnScreen = num;
}




void detectPlayerEnemyCollisions()
{
	/*Enemy *temp = e;
	if (player->Position.Y < 1.5)
	{
		while(temp != 0)
		{
			if (temp->Translation.X - temp->size < player->Position.X && temp->Translation.X + temp->size > player->Position.X)
			{
				if (temp->Translation.Z - temp->size < player->Position.Z && temp->Translation.Z + temp->size > player->Position.Z)
				{
					if (temp->alive)
					{
						numEnemiesKilled++;
						numEnemiesOnScreen--;
				
						temp->alive = 0;
					}				
				}
			}
			temp = temp->next;
		}
	}
	checkNumEnemies();*/
}


void detectEnemyEnemyCollisions(float time)
{
	Enemy *temp = e;
   	while(temp != 0)
	{
		temp->update(time);
		
		Enemy *tocheck = e;
		while (tocheck != 0)
		{
			if (temp != tocheck)
			{
				if (checkProximity(temp, tocheck))
				{
					temp->update(-time);
					break;
				}
			}
			tocheck = tocheck->next;
		}

		temp = temp->next;
	}
}


/***************************
 * GLUT Callback Functions *
 ***************************/

// OpenGL initialization
void Initialize()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Start keeping track of time
	Time0 = glutGet(GLUT_ELAPSED_TIME);
}

void initEnemies()
{
	e = new Enemy(rand());
}

void update(float dtime)
{
	//player->update(dtime, w, a, s, d);
	camera->update();

	if (shouldAddEnemy()) addEnemy();

	detectEnemyEnemyCollisions(dtime);
	//detectPlayerEnemyCollisions();
}

// Manages time independant movement and draws the VBO
void Display()
{

	// Determine time since last draw
	Time1 = glutGet(GLUT_ELAPSED_TIME);
	float Delta = (float) (Time1 - Time0) / 1000.f;
	Time0 = Time1;

	if (curTime < 60000)
        {
	    update(Delta);

	    int dtime = (time(NULL) - *timeTracker) * 1000;
	    curTime += dtime;
	    numFrames++;
            if (curTime / 1000 > lastSecond)
	    {
		    lastSecond++;
		    FPS = numFrames;
		    numFrames = 0;
	    }
	    time(timeTracker);

  }

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawText();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera->setLookAt();

	Enemy *temp = e;
    	while(temp != 0)
	{
		temp->draw();
		temp = temp->next;
	}
	map->draw();

	glutSwapBuffers();
	glutPostRedisplay();
}

void Reshape(int width, int height)								
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));
	WindowWidth = width;
	WindowHeight = height;

	// Set camera projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float AspectRatio = (float)WindowWidth / (float)WindowHeight;
	gluPerspective(60.0, AspectRatio, 0.01, 1000.0);
}

void keyCallback(unsigned char key, int x, int y) {
   
   if (key == 'w')
	   w = 1;
   if (key == 's')
	   s = 1;
   if (key == 'a')
	   a = 1;
   if (key == 'd')
	   d = 1;
}

void keyUpCallback(unsigned char key, int x, int y) {
   if (key == 27)
	   exit(0);

   if (key == 'w')
	   w = 0;
   if (key == 's')
	   s = 0;
   if (key == 'a')
	   a = 0;
   if (key == 'd')
	   d = 0;
}

void mouseMotion(int x, int y)
{
	if (curTime < 60000)
	{
	    int dx = prevX - x;
	    int dy = prevY - y;
	}
}


int main(int argc, char * argv[])
{
	glutInit(& argc, argv);
 	glutInitWindowPosition(100, 200);
 	glutInitWindowSize(WindowWidth, WindowHeight);
 	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

 	glutCreateWindow("476 Lab 1: Make Haste!");
	glutReshapeFunc(Reshape);
 	glutDisplayFunc(Display);
	
  timeTracker = (time_t*)malloc(sizeof(time_t));
  *timeTracker = time(timeTracker);

	prevX = 200;
	prevY = 200;
	glutSetCursor(GLUT_CURSOR_NONE); 
	
	glutKeyboardFunc(keyCallback);
	glutKeyboardUpFunc(keyUpCallback);
 	glutPassiveMotionFunc(mouseMotion);

 	Initialize();

	camera = new Camera(6, 4, 3);
	map = new Map();
	initEnemies();

	// ... and begin!
	glutMainLoop();
	
	return 0;
}
