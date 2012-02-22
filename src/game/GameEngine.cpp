#include "GameEngine.h"
#include "Object3d.h"
#include "Player.h"
#include "SceneObject.h"

#define VELOCITY_CONSTANT 2

GameEngine::GameEngine(Modules *modules) {
   m_modules = modules;

   gameOver = 0;
   
   InitData();
}


GameEngine::~GameEngine() 
{
   
}

void GameEngine::InitData()
{
   m_worldData = m_modules->resourceManager->readWorldData("maps/course.wf");
   
   m_worldGrid = new WorldGrid(*m_worldData, m_modules);
   
   PathPoint currPath = m_worldGrid->getCurrentQuadrant().m_startPt;
   PathPoint prevPath = m_worldGrid->getCurrentQuadrant().m_endPt;
   
   m_camera = new Camera(&currPath, &prevPath);
   m_player = new Player("models/spaceship.obj", "textures/test3.bmp", 
                         m_modules, m_camera->getPosition(), 
                         m_camera->getForward(), m_camera->getUp());
   m_camera->setPlayer(m_player);
   
   m_worldGrid->setPlayer(m_player);
   
   m_reticle = new Reticle("models/reticle2.obj", "textures/test3.bmp", 
                           m_modules, m_player);
   
   m_enemyShip = new EnemyShip("models/enemy.obj", "textures/test3.bmp", 
			       m_modules, *m_player);
   /*m_enemyGunner = new EnemyGunship("models/enemy2.obj", "models/enemy2turretbase.obj",
          "models/enemy2turrethead.obj", "textures/test3.bmp", m_modules, *m_player);*/
   
   m_player->setProgress(prevPath.getPosition());
   m_player->setPosition(prevPath.getPosition());
   m_player->setUp(prevPath.getUp());
   m_player->setHeads(currPath.getPosition(), 
		      currPath.getUp(), prevPath.getPosition(), 
		      prevPath.getUp());
   m_player->calculateSide();
   
   m_enemyShip->setProgress(prevPath.getPosition());
   m_enemyShip->setPosition(prevPath.getPosition());
   m_enemyShip->setUp(prevPath.getUp());
   m_enemyShip->setHeads(currPath.getPosition(), 
			 currPath.getUp(), prevPath.getPosition(), 
			 prevPath.getUp());
   m_enemyShip->calculateSide();
   
   m_modules->renderingEngine->setCamera(m_camera);

   m_enemyShip->setPosition(m_player->getPosition() + (m_player->getForward() * 10000));
   
   m_worldGrid->placeInCurrQuadrant(m_player, m_player);
   //m_worldGrid->placeInGrid(m_enemyShip, m_enemyShip);
   
   initSound();
   m_bulletSound = loadSound("sound/arwingShot.ogg");
   m_music = loadMusic("sound/venom.mp3");
   
   //m_music->play(1);
}

void GameEngine::tic(uint64_t td) {
   gameOver += td;
   
   if (gameOver >= 40000) 
   {
      cout << "YOU WIN!\n";
      exit(0);
   }
   
   if (!m_player->isAlive()) {
      cout << "YOU LOSE!\n";
      exit(0);
   }
   
   PathPointData currPPD = m_worldGrid->getCurrentQuadrant().m_startPt;
   
   PathPoint m_currentPoint(currPPD);
   
   // Update functions go here

//   m_enemyShip->setBearing(m_currentPoint.getPosition(), m_currentPoint.getUp());
//   m_enemyShip->tic(td);
   
   m_camera->checkPath(&m_currentPoint);
   m_camera->tic(td);
   
   m_worldGrid->tic(td);
   m_reticle->tic(td);

   m_worldGrid->checkCollisions();
}


void GameEngine::render() {
   list<IObject3d*> objs = m_worldGrid->getDrawableObjects();
   
   m_modules->renderingEngine->render(objs);
}


// Take care of mouse and keyboard events
bool GameEngine::handleEvents()
{
   bool running = true;
   
   SDL_Event evt;
   
   
   while (SDL_PollEvent(&evt))
   {
      
      if (evt.type == SDL_QUIT)
         running = false;
      
      // Keyboard events
      if (evt.type == SDL_KEYUP)
      {
         running = handleKeyUp(evt.key.keysym.sym);
      }

      if (evt.type == SDL_KEYDOWN) {
	 running = handleKeyDown(evt.key.keysym.sym);
      }
      
      // Mouse Events
      if (evt.type == SDL_MOUSEMOTION) 
      {
         handleMouseMotion(evt.motion.x, evt.motion.y);
      }
      
      //   if (evt.type == SDL_MOUSEBUTTONUP)
      //   {
      //      mouse_click(evt.button.button);
      //   }
   }
      
   return running;
}

bool GameEngine::handleKeyDown(SDLKey key) {
   bool running = true;

   if (key == SDLK_SPACE) {
      m_camera->setBoosting(true);
      m_reticle->setVisible(false);
   }
   
   if (key == SDLK_z)
   {
      if (!m_camera->isBoosting()) {
         Bullet *bullet = new Bullet("models/cube.obj", "textures/test4.bmp", 
           m_modules, m_player->getPosition() 
           + (m_player->getSide() * 8),
           m_player->getAimForward(), m_player->getAimUp(), 
           *m_player, Bullet::defaultTimeToLive, 1.0f);
          
         // HACK so that I can use it as both a GameObject and an Object3d
         m_worldGrid->placeInCurrQuadrant(bullet, bullet);
          
         bullet = new Bullet("models/cube.obj", "textures/test4.bmp", 
                    m_modules, m_player->getPosition() 
                    - (m_player->getSide() * 8),
                    m_player->getAimForward(), m_player->getAimUp(), 
                    *m_player, Bullet::defaultTimeToLive, 1.0f);
            
         // HACK so that I can use it as both a GameObject and an Object3d
         m_worldGrid->placeInCurrQuadrant(bullet, bullet);
         
	 //m_bulletSound->play(0);
      }
   }

   return running;
}

std::vector<GameObject*> GameEngine::acquireMissileTargets() {
   std::vector<GameObject*> temp;
   vec3 playerToObjVec;
   int count = 0;
   
   for (list<GameObject *>::const_iterator it = 
        m_worldGrid->getCurrentQuadrant(). m_gameObjects.begin(); 
        it != m_worldGrid->getCurrentQuadrant().m_gameObjects.end(); it++) 
   {
      if (typeid(**it) != typeid(Bullet) && typeid(**it) != typeid(Player) && typeid(**it) != typeid(Missile)) {
         playerToObjVec = (*it)->getPosition() - m_player->getPosition();
         if (playerToObjVec.Length() > 350 && 
             playerToObjVec.Length() < 1500 && 
             angleBetween(m_player->getAimForward(), playerToObjVec) < 60.0f) {
            temp.push_back(*it);
            count++;
            if (count == 6) {
               return temp;
            }
         }
      }
   }

  return temp;
}

ICamera& GameEngine::getCamera()
{
   return *m_camera;
}

float GameEngine::angleBetween(vec3 one, vec3 two) {
  return 180.0f / 3.14159265f *
    acos(one.Normalized().Dot(two.Normalized()));
}

bool GameEngine::handleKeyUp(SDLKey key)
{
   bool running = true;
   std::vector<GameObject*> targets;
   vec3 curveDir, bulletOrigin;
   
   if (key == SDLK_ESCAPE) 
   {
      running = false;
   }
   
   if (key == SDLK_x) {
      if (!m_camera->isBoosting()) {
         targets = acquireMissileTargets();
         
         //For each target
         for (int index = 0; index < targets.size(); index++) {
            switch (index) {
               case 0:
                  //AIM UP OR CAMERA UP?
                  curveDir = ((m_player->getSide() * .75)
                              - (m_player->getAimUp() * .35)).Normalized();
                  bulletOrigin = m_player->getSide() * 20;
                  break;
               case 1:
                  curveDir = ((m_player->getSide() * -.75) 
                              - (m_player->getAimUp() * .35)).Normalized();
                  bulletOrigin = m_player->getSide() * -20;
                  break;
               case 2:
                  curveDir = ((m_player->getSide() * .75) 
                              + (m_player->getAimUp() * .35)).Normalized();
                  bulletOrigin = m_player->getSide() * 20;
                  break;
               case 3:
                  curveDir = ((m_player->getSide() * -.75) 
                              + (m_player->getAimUp() * .35)).Normalized();
                  bulletOrigin = m_player->getSide() * -20;
                  break;
               case 4:
                  curveDir = m_player->getSide();
                  bulletOrigin = m_player->getSide() * 20;
                  break;
               case 5:
                  curveDir = m_player->getSide() * -1.0f;
                  bulletOrigin = m_player->getSide() * -20;
            }
            
            bulletOrigin += (m_player->getAimForward() * 8.0f);
            
            Missile *missile = new Missile("models/cube.obj", 
                                           "textures/test6.bmp",
                                           m_modules, 
                                           bulletOrigin, 
                                           m_player->getAimForward(), 
                                           curveDir,
                                           m_player, 
                                           targets.at(index));
            
            m_worldGrid->placeInCurrQuadrant(missile, missile);
         }
      }
   }
      
   if (key == SDLK_F1) {
      m_camera->setCameraType(_MOTION_CAMERA);
   }
   
   if (key == SDLK_F2) {
      m_camera->setCameraType(_PATH_CAMERA);
   }

   if (key == SDLK_F3) {
      m_camera->setCameraType(_SHIP_CAMERA);
   }
   
   if (key == SDLK_F4) {
      m_camera->setCameraType(_FPS_CAMERA);
   }
   
   if (key == SDLK_SPACE) {
      m_camera->setBoosting(false);
      m_reticle->setVisible(true);
   }
   
   return running;
}

void GameEngine::handleMouseMotion(Uint16 x, Uint16 y)
{
   // Rotate player?
   // X seems to be reading in backwards...?
   m_player->updateVelocity(400-x, 300-y);
   //setVelocity(vec3((400 - x), (300 - y), m_player->getPosition().z));
}
