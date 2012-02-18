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
   
private:
   void runCollisions();
   void createTurrets();
   
//   list<IObject3d *> m_objects;
//   list<GameObject *> m_gameObjects;
//   Player *m_player;
//   Reticle *m_reticle;
//   Camera *m_camera;
//   Modules *m_modules;
   Path *m_world;
//   WorldData *m_turretLocs;
//   PathPoint *m_currentPoint;
//   PathPoint *m_previousPoint;
   Vector3<float> currentDirection;
   void InitData();
//   std::vector<Bullet *> m_bulletList;
//   std::vector<Turret *> m_turrets;
//   std::vector<Missile *> m_missileList;
   
//   EnemyShip* m_enemyShip;
   //EnemyGunship* m_enemyGunner;
   
   Sound* m_bulletSound;
   Sound* m_music;
   
//   Explodeable* explosion;
   uint64_t gameOver;
};

#endif
