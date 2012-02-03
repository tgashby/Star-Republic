#ifndef StarRepub_GameEngine_h
#define StarRepub_GameEngine_h

#include "Interfaces.h"
#include "Camera.h"

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

#ifdef __unix__
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

#ifdef _WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#endif

class GameEngine : public IGameEngine {
public:
   GameEngine(Modules *modules);
   ~GameEngine();
   void tic(unsigned int td);
   void render();
   
   bool handleEvents();
   bool handleKeyUp(SDLKey key);
   void handleMouseMotion(Uint16 x, Uint16 y);
   
private:
   list<IObject3d *> m_objects;
   Camera *m_camera;
   Modules *m_modules;
};

#endif
