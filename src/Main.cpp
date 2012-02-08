/* SDL */
#include "engine/SDL_include.h"

#include "engine/Interfaces.h"
#include "engine/ResourceManager.h"
#include "game/GameEngine.h"
#include "engine/RenderingEngine.h"

bool handleEvents(float dt, SDL_Event& evt);
bool handleKeyUp(SDLKey key);
void handleMouseMotion(Uint16 x, Uint16 y);

int main(int argc, char** argv)
{
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
   
   // Setup the modules
   Modules *modules = new Modules();
   modules->resourceManager = new ResourceManager();
   modules->renderingEngine = new RenderingEngine(modules);
   modules->gameEngine = new GameEngine(modules);
   
   // Swap buffers to display what you've drawn
   SDL_GL_SwapBuffers();
   
   bool running = true;
   uint64_t now, then1, then2;
   then1 = SDL_GetTicks();
   while(running)
   {
      // Handle Events (keyboard, mouse, etc)
      running = modules->gameEngine->handleEvents();
      then2 = SDL_GetTicks();
      now = then2 - then1;
      modules->gameEngine->tic(now);
      modules->gameEngine->render();
      SDL_GL_SwapBuffers();
      then1 = then2;
   }
   
   SDL_Quit();
   
   delete modules;
   
   return 0;
}

