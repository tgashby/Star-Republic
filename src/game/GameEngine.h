#ifndef StarRepub_GameEngine_h
#define StarRepub_GameEngine_h

#include "Interfaces.h"
#include "Camera.h"
#include "ResourceManager.h"

#include "SDL_include.h"
#include "Path.h"
#include "PathPoint.h"
#include "Turret.h"
#include "Player.h"
#include "Reticle.h"
#include <assert.h>
#include "Bullet.h"
#include "EnemyShip.h"
//#include "EnemyGunship.h"
#include "WorldGrid.h"
#include "Explodeable.h"
#include <algorithm>
#include "Missile.h"


class GameEngine : public IGameEngine {
public:
   GameEngine(Modules *modules);
   ~GameEngine();
   void tic(uint64_t td);
   void render();
   
   bool handleEvents();
   bool handleKeyUp(SDLKey key);
   bool handleKeyDown(SDLKey key);
   void handleMouseMotion(Uint16 x, Uint16 y);
   float angleBetween(vec3 one, vec3 two);
   std::vector<GameObject*> acquireMissileTargets();
   ICamera& getCamera();
   
private:
   void runCollisions();
   
   Camera *m_camera;
   Player *m_player;
   Reticle *m_reticle;
   EnemyShip* m_enemyShip;
   Modules *m_modules;
   WorldData *m_worldData;
   WorldGrid *m_worldGrid;
   Vector3<float> currentDirection;
   void InitData();
   
   Sound* m_bulletSound;
   Sound* m_music;
   
   uint64_t gameOver;
};

#endif
