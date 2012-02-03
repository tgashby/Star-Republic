#ifndef StarRepub_GameEngine_h
#define StarRepub_GameEngine_h

#include "Interfaces.h"
#include "Camera.h"

class GameEngine : public IGameEngine {
public:
   GameEngine(Modules *modules);
   ~GameEngine();
   void tic(unsigned int td);
   void render();
private:
   list<IObject3d *> m_objects;
   Camera *m_camera;
   Modules *m_modules;
};

#endif
