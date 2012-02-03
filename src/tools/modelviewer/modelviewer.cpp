
/* SDL */
#ifdef _WIN32
	#include <SDL.h>
	#include <SDL_opengl.h>
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_opengl.h>
#endif


#include "Interfaces.h"
#include "ResourceManager.h"
#include "GameEngine.h"
#include "RenderingEngine.h"

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
   
   SDL_WM_SetCaption("Star Republic", NULL);
   
   // Boring OGL
   glPushMatrix();
   
   //glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);
   
   glPopMatrix();
   
   // Setup the modules
   Modules *modules = new Modules();
   modules->resourceManager = new ResourceManager();
   modules->renderingEngine = new RenderingEngine(modules);
   modules->gameEngine = new GameEngine(modules);
   
   
   // Swap buffers to display what you've drawn
   SDL_GL_SwapBuffers();
   
   SDL_Event myEvent;
   bool running = true;
   
   while(running){
      SDL_PollEvent(&myEvent);
      
      if (myEvent.type == SDL_QUIT)
         running = false;
      
      modules->gameEngine->render();
      SDL_GL_SwapBuffers();
   }
   
   SDL_Quit();
   return 0;
}
