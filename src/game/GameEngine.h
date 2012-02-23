#ifndef StarRepub_GameEngine_h
#define StarRepub_GameEngine_h

#include "Interfaces.h"
#include "Camera.h"
#include "StateManager.h"
#include "ResourceManager.h"
#include "RenderingEngine.h"

#include "SDL_include.h"
#include "Path.h"
#include "PathPoint.h"
#include "Turret.h"
#include "Player.h"
#include "Reticle.h"
#include <assert.h>
#include "Bullet.h"
#include "EnemyShip.h"
#include "EnemyGunship.h"
#include "Explodeable.h"
#include <algorithm>
#include "Missile.h"
#include "GameState.h"
#include "MenuState.h"
#include "Asteroid.h"


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
   void createTerrain();
   void addAsteroids();
   
   list<IObject3d *> m_objects;
   list<GameObject *> m_gameObjects;
   Player *m_player;
   Reticle *m_reticle;
   Camera *m_camera;
   Modules *m_modules;
   StateManager *m_stateManager;
   MenuState* m_menu;
   GameState* m_game;
   Path *m_path;
   WorldData *m_worldData;
   PathPoint *m_currentPoint;
   PathPoint *m_previousPoint;
   Vector3<float> currentDirection;
   void InitData();
   std::vector<Bullet *> m_bulletList;
   std::vector<Turret *> m_turrets;
   std::vector<Missile *> m_missileList;
   
   EnemyShip* m_enemyShip;
   EnemyGunship* m_enemyGunner;
   
   Sound* m_bulletSound;
   Sound* m_missileSound;
   Sound* m_music;
   Sound* m_boostSound;
   
   Explodeable* explosion;
   uint64_t gameOver;
};

#endif
