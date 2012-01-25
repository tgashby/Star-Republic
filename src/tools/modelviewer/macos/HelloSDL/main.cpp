#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "main.h"
#include "ApplicationEngine.h"
#include "RenderingEngine.h"
#include "ResourceManager.h"

#define WIDTH 640
#define HEIGHT 480
#define WINDOW_TITLE "Model Viewer"

SDL_Surface *screen;

int SDL_main (int argc, char **argv) {
   // Initialize
   SDL_Init(SDL_INIT_VIDEO);
   
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
   
   // Enable double-buffering
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   
   // Create a OpenGL window
   screen = SDL_SetVideoMode(WIDTH, HEIGHT, 0, SDL_OPENGL | 
                             SDL_RESIZABLE);
   if(!screen) 
   {
      printf("Couldn't set %dx%d GL video mode: %s\n", WIDTH,
             HEIGHT, SDL_GetError());
      SDL_Quit();
      exit(2);
   }
   SDL_WM_SetCaption(WINDOW_TITLE, WINDOW_TITLE); 
   
   // Create App
   ResourceManager *resourceManager = new ResourceManager();
   RenderingEngine *renderEngine = new RenderingEngine(resourceManager);
   ApplicationEngine *application = new ApplicationEngine(resourceManager, renderEngine);
   
   // Get the time
   unsigned int prevTime = SDL_GetTicks();
   unsigned int curTime = prevTime;
   
   
   // Start Main Loop
   bool done = false;
   while(!done) 
   {
      SDL_Event event;
      
      // Respond to any events that occur
      while(SDL_PollEvent(&event))
      {
         switch(event.type) 
         {
            case SDL_VIDEORESIZE:
               screen = SDL_SetVideoMode(event.resize.w, 
                                         event.resize.h, 0,
                                         SDL_OPENGL | SDL_RESIZABLE);
               if(screen)
               {
                  // reshape(screen->w, screen->h);
               } 
               else 
               {
                  ; // Oops, we couldn't resize for some reason. 
                  // This should never happen
               }
               break;
               
            case SDL_QUIT:
               done = true;
               break;		
               
            case SDL_KEYDOWN:
               application->handleKeyDown(SDL_GetKeyName(event.key.keysym.sym));
               break;
               
            case SDL_KEYUP:
               application->handleKeyUp(SDL_GetKeyName(event.key.keysym.sym));
               break;
               
            case SDL_MOUSEMOTION:
               application->handleMouseMove(event.motion.xrel, event.motion.yrel);
               break;
               
            case SDL_MOUSEBUTTONDOWN:
               application->handleMouseDown(event.button.button, event.button.x, event.button.y);
               break;
               
            case SDL_MOUSEBUTTONUP:
               application->handleMouseUp(event.button.button, event.button.x, event.button.y);
               //printf("Mouse button %d up at (%d,%d)\n",
               //       event.button.button, event.button.x, event.button.y);
               break;
            //case SDL_MOUSEWHEEL:
               //application->handelMouseWheel(event.wheel.y);
         }
      }
      
      curTime = SDL_GetTicks();
      application->tic(curTime - prevTime);
      
      /*
       //Check for escape
       Uint8 *keys = SDL_GetKeyState(NULL);
       if( keys[SDLK_ESCAPE] ) {
       done = true;
       }*/
      
      application->render();
      
      SDL_GL_SwapBuffers();
      prevTime = curTime;
   }
   
   // Clean up the application
   delete application;
   
   SDL_Quit();
   return 0;
}