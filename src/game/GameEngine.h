#ifndef StarRepub_GameEngine_h
#define StarRepub_GameEngine_h

#include "Interfaces.h"
#include "Camera.h"
#include "ResourceManager.h"

#include "SDL_include.h"

#define VELOCITY_CONSTANT 2
#include "World.h"
#include "WorldPoint.h"
#include "Turret.h"
#include "Player.h"
#include <assert.h>
#include "Bullet.h"


class GameEngine : public IGameEngine {
public:
   GameEngine(Modules *modules);
   ~GameEngine();
   void tic(uint64_t td);
   void render();
   
   bool handleEvents();
   bool handleKeyUp(SDLKey key);
   void handleMouseMotion(Uint16 x, Uint16 y);
   
private:
   list<IObject3d *> m_objects;
   Player *m_player;
   Camera *m_camera;
   Modules *m_modules;
   World *m_world;
   Turret *m_turret;
   WorldPoint *m_currentPoint;
   WorldPoint *m_previousPoint;
   Vector3<float> currentDirection;
   void InitData();
   std::vector<Bullet *> m_bulletList;
   
   Sound* m_bulletSound;
   Sound* m_music;
};

#endif
