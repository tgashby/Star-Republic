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
//#include "EnemyGunship.h"
#include "WorldGrid.h"
#include "Explodeable.h"
#include <algorithm>
#include "Missile.h"
#include "GameState.h"
#include "MenuState.h"
#include "WinState.h"
#include "LoseState.h"
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
   ICamera& getCamera();
   
private:
   void runCollisions();
//   void addAsteroids();
   
   Camera *m_camera;
   Player *m_player;
   Reticle *m_reticle;
   Modules *m_modules;
   StateManager *m_stateManager;
   MenuState* m_menu;
   GameState* m_game;
<<<<<<< HEAD
=======
   LoseState* m_lose;
   WinState* m_win;
   Path *m_path;
>>>>>>> 6f97755cbb180aa2f235d770d0b1473696d1c8bd
   WorldData *m_worldData;
   WorldGrid *m_worldGrid;
   Vector3<float> currentDirection;
   void InitData();
<<<<<<< HEAD
   
   EnemyShip* m_enemyShip;
   EnemyGunship* m_enemyGunner;
=======
   std::vector<Bullet *> m_bulletList;
   std::vector<Turret *> m_turrets;
   std::vector<Missile *> m_missileList;

   std::vector<EnemyShip *> m_enemyShips;
   std::vector<EnemyGunship*> m_enemyGunners;
>>>>>>> 6f97755cbb180aa2f235d770d0b1473696d1c8bd
   
   Sound* m_bulletSound;
   Sound* m_missileSound;
   Sound* m_music;
   Sound* m_boostSound;
   
   uint64_t gameOver;
};

#endif
