//
//  main.cpp
//
//  Created by Taggart Ashby on 1/10/12.
//

/* SDL */
#ifdef _WIN32
	#include <SDL.h>
	#include <SDL_opengl.h>
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_opengl.h>
#endif

#include <iostream>
#include <list>
//#include "GameObject.h"

int Time0, Time1;
//std::list<GameObject> Others;

int main(int argc, char** argv){
   SDL_Surface* screen;
   
   // Call SDL_INIT(SDL_EVERYTHING)
   SDL_Init(SDL_INIT_EVERYTHING);
   
   // Set the SDL GL Attributes
   SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
   SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   
   // Create the window
   screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE | SDL_OPENGL);
   
   SDL_WM_SetCaption("Test!", NULL);
   
   // Boring OGL
   glPushMatrix();
   
   glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);
   
   glPopMatrix();
   //makeContent();
   //gameLoop();
   
   // Swap buffers to display what you've drawn
   SDL_GL_SwapBuffers();
   
   SDL_Event myEvent;
   bool running = true;
   
   while(running){
      SDL_PollEvent(&myEvent);
      
      if (myEvent.type == SDL_QUIT)
         running = false;
   }
   
   SDL_Quit();
   return 0;
}

void gameLoop()
{
   while(true) {
      //update();
      //draw();
   }
}

void makeContent()
{
   //player = new Player(blah, blah, blah);
   //world = new World();
   //set camera
   //fill the enemy list
   //set the current terrain and line
   //Player = new GameObject(
}


/*
void update()
{
   int dt;
   Time1 = SDL_GetTicks();
   dt = Time1 - Time0;
   Time0 = Time1;

   Ship.tic(dt);
   for (std::list<GameObject>::iterator i = Others.begin();
         i != Others.end(); i++) {
      i.tic(dt);
   }

   for (std::list<GameObject>::iterator i = Others.begin(); 
        i != Others.end(); i++) {
      for (std::list<GameObject>::iterator j = i; j != Others.end(); j++) {
         Collision::collisionCheck(i, j);
      }
   }

   
}

*/
