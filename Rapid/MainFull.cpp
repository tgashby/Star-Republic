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


// Utility classes for loading shaders/meshes
#include "CShader.h"
#include "CMeshLoader.h"
#include "Enemy.h"
#include "Camera.h"
#include "Map.h"
#include "HUD.h"
#include "InputManager.h"
#include "Player.h"
#include "Bullets.h"
//#include "Bullet.h"
#include "Collision.h"
#include "GameObject.h"
#include "Turrets.h"
#include "Turret.h"
#include "Util/Vector.hpp"

#define LOOK_SPEED 0.1


/********************
 * Global Varaibles *
 ********************/


// Window information
int WindowWidth = 1000, WindowHeight = 600;

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
Player *player;
Map *map;
HUD* hud;
InputManager* manager;
Bullets* bullets;
Turrets* turrets;


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

}

void update(float dtime)
{
	player->update(dtime, map);
  SVector3* temp = new SVector3();

  int i = 0;
  temp->X = 0;
  temp->Y = 0;
  temp->Z = 5.0;
  SVector3* temp2 = new SVector3();
  temp2->X = -player->getPosition()->X;
  temp2->Y = player->getPosition()->Y;
  temp2->Z = player->getPosition()->Z;
  vec3 aim = vec3(0 - player->getVelocity()->X / 3, 0 + player->getVelocity()->Y / 3, 5.0);
  aim.Normalize();
  temp->X = aim.x * 8.0;
  temp->Y = aim.y * 8.0;
  temp->Z = aim.z * 8.0;
  //printf("\nThe aim is %f, %f, %f.\n", temp->X, temp->Y, temp->Z);
  if (player->canFire()) {
     bullets->addBullet(temp2, temp, NULL, 1.0, 10);
     i = 1;
  }
  turrets->shootIfPossible();
  Turret* current = turrets->first;
  while (current != NULL) {
     if (current->firing == true && current->cooldown == 0 && current->health > 0) {
        current->cooldown = 50;
        temp->X = -current->Translation.X;
        temp->Y = current->Translation.Y;
        temp->Z = current->Translation.Z;
        //bullets->addBulletBad(temp, current->firingDirection, NULL, 1.0, 10);
     }
     current = current->next;
  }
  camera->update();
  turrets->update(dtime);
  bullets->update(dtime, map);
  /**COLLISION CALLS HERE**/
  bullets->collideWith(player);
  bullets->collideWith(turrets);
  bullets->removeDead(camera->getPosition());

	turrets->collideAllWith(player);
}
   
// Manages time independent movement and draws the VBO
void Display()
{
	// Determine time since last draw
	Time1 = glutGet(GLUT_ELAPSED_TIME);
	float Delta = (float) (Time1 - Time0) / 1000.f;
	Time0 = Time1;

  
  if (player->health > 0 && curTime < 120000)
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

   	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	   hud->drawText(FPS, curTime, player->health, turrets->countAll());
//     hud->renderGlutAimer(player->getPosition()->X, player->getPosition()->Y, manager->AbsX, manager->AbsY);

	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();

	   camera->setLookAt();

	   map->draw();
     player->draw();
     turrets->drawAll();
     bullets->draw();
     
     hud->drawText(FPS, curTime, player->health, turrets->countAll());
     hud->renderGlutAimer(player->getPosition()->X, player->getPosition()->Y, manager->AbsX, manager->AbsY);


  }
  else if (curTime >= 120000)
  {
	   hud->drawText(FPS, curTime, player->health, turrets->countAll());
     hud->drawWin();
  }
  else
  {
	   hud->drawText(FPS, curTime, 0, turrets->countAll());
     hud->drawLose();
  }
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
  manager->keyCallBack(key, x, y);
}

void keyUpCallback(unsigned char key, int x, int y) {
  manager->keyUpCallBack(key, x, y);
}

void mouseMotion(int x, int y)
{
	manager->mouseMotion(x,y);
}


int main(int argc, char * argv[])
{
	glutInit(& argc, argv);
 	glutInitWindowPosition(100, 200);
 	glutInitWindowSize(WindowWidth, WindowHeight);
 	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

 	glutCreateWindow("Star Republic - The Destruction Of Helios");
	glutReshapeFunc(Reshape);
 	glutDisplayFunc(Display);
	
  timeTracker = (time_t*)malloc(sizeof(time_t));
  *timeTracker = time(timeTracker);

	prevX = 200;
	prevY = 200;
	//glutSetCursor(GLUT_CURSOR_NONE); 
	

 	Initialize();
  float size = 1.0;

	map = new Map();
	player = new Player(new SVector3(0,0,6), new SVector3(0,0,4), NULL, size);
	camera = new Camera(6, 4, 3, player,map);
  manager = new InputManager(player);
  turrets = new Turrets(map, 20, player);
	hud = new HUD();
  bullets = new Bullets();
	initEnemies();

  manager->sendPlayerPositionPulse();      

	glutKeyboardFunc(keyCallback);
	glutKeyboardUpFunc(keyUpCallback);
 	glutPassiveMotionFunc(mouseMotion);
	// ... and begin!
	glutMainLoop();
	
	return 0;
}
