#include "GameEngine.h"
#include "Object3d.h"
#include "Player.h"
#include "SceneObject.h"

#define VELOCITY_CONSTANT 2

GameEngine::GameEngine(Modules *modules) {
   m_modules = modules;

   gameOver = 0;
   m_stateManager = new StateManager();
   m_menu = new MenuState();
   m_game = new GameState();
   m_game->initialize();
   m_menu->initialize();
   m_stateManager->pushState(m_game);
   m_stateManager->pushState(m_menu);

   //INIT DATA not being called, only called when the menu is left.
   //InitData();
}


GameEngine::~GameEngine() 
{
   
}

void GameEngine::InitData()
{
   m_worldData = m_modules->resourceManager->readWorldData("maps/course3.wf");
   
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
			       m_modules, m_player);
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

   m_enemyGunner->setProgress(prevPath.getPosition());
   m_enemyGunner->setPosition(prevPath.getPosition());
   m_enemyGunner->setUp(prevPath.getUp());
   m_enemyGunner->setHeads(currPath.getPosition(), 
			 currPath.getUp(), prevPath.getPosition(), 
			 prevPath.getUp());
   m_enemyGunner->calculateSide();
   
   m_modules->renderingEngine->setCamera(m_camera);

   m_enemyShip->setPosition(m_player->getPosition() + (m_player->getForward() * 10000));
   
   m_worldGrid->placeInCurrQuadrant(m_player, m_player);
//   m_worldGrid->placeInGrid(m_enemyShip, m_enemyShip);
//   m_worldGrid->placeInGrid(m_enemyGunner, m_enemyGunner);
   
   initSound();
   m_bulletSound = loadSound("sound/arwingShot.ogg");
   m_music = loadMusic("sound/venom.mp3");
   //addAsteroids();
   //m_music->play(1);
}

void GameEngine::tic(uint64_t td) {
   //CHECKS TO MAKE SURE THE CURRENT STATE IS A GAME STATE. THIS SHOULD PROBABLY BE MODIFIED TO SOMETHING MORE ELEGANT.
   if (m_stateManager->getCurrentState() == m_game)
   {
      gameOver += td;
      
      /*
      if (gameOver >= 40000) 
      {
         cout << "YOU WIN!\n";
         exit(0);
      }
       */
      
      PathPointData currPPD = m_worldGrid->getCurrentQuadrant().m_startPt;
      
      PathPoint m_currentPoint(currPPD);
      
      // Update functions go here

//      m_enemyShip->setBearing(m_currentPoint.getPosition(), m_currentPoint.getUp());
//      m_enemyShip->tic(td);
//
//      m_enemyGunner->setBearing(m_currentPoint.getPosition(), m_currentPoint.getUp());
//      m_enemyGunner->tic(td);
      
      m_camera->checkPath(&m_currentPoint);
      m_camera->tic(td);
      
      m_worldGrid->tic(td);
      m_reticle->tic(td);

      m_worldGrid->checkCollisions();
   }
}


void GameEngine::render() {
   if (m_stateManager->getCurrentState() == m_game) 
   {
      list<IObject3d*> objs = m_worldGrid->getDrawableObjects();
      
      m_modules->renderingEngine->render(objs);
   }
   
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

//void GameEngine::addAsteroids() {
//   Asteroid* tempAst;
//   PathPoint* current;
//   PathPoint* prev;
//   for (int pntIndex = 1; pntIndex < m_path->getSize(); pntIndex++) {
//      current = &(m_path->getAt(pntIndex));
//      prev = &(m_path->getAt(pntIndex - 1));
//      //ADD AN ASTEROID
//      tempAst = new Asteroid("models/sphere.obj", "textures/test4.bmp", 
//			     m_modules, current->getPosition(), current->getUp(), 			     prev->getPosition() - current->getPosition());
//      m_modules->renderingEngine->addObject3d(tempAst);
//      m_gameObjects.push_back(tempAst);
//      m_objects.push_back(tempAst);
//      tempAst = new Asteroid("models/cube.obj", "textures/test5.bmp",
//			     m_modules, current->getPosition() + (current->getUp() * 10.0f), current->getUp(), current->getForward());
//      m_modules->renderingEngine->addObject3d(tempAst);
//      m_gameObjects.push_back(tempAst);
//      m_objects.push_back(tempAst);
//   }
//}

bool GameEngine::handleKeyDown(SDLKey key) {
   bool running = true;

   //Checks to see whether the current state is the menu, and pops the state if so. Will be revised later.
   if (m_stateManager->getCurrentState() == m_menu)
   {
      m_stateManager->popState();
      InitData();
      return running;
   }
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
   
   Quadrant quad = m_worldGrid->getCurrentQuadrant();
   
   for (list<GameObject *>::iterator it = quad.m_gameObjects.begin(); 
        it != quad.m_gameObjects.end(); it++) {
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

   //Checks if the current state is the "menu", if so it closes the main menu to start the game state. Could be improved.
   if (m_stateManager->getCurrentState() == m_menu)
   {
      m_stateManager->popState();
      InitData();
      return running;
   }
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
	    
	    Missile *missile = new Missile("models/cube.obj", "textures/test6.bmp",
					   m_modules, 
					   bulletOrigin, 
					   m_player->getAimForward(), 
					   curveDir,
					   m_player, 
					   targets.at(index));
	    
	    // HACK so that I can use it as both a GameObject and an Object3d
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
   //Checks if the current state is a game state. If so, reads in the mouse motion.
   if (m_stateManager->getCurrentState() == m_game)
   {
   // Rotate player?
   // X seems to be reading in backwards...?
   m_player->updateVelocity(400-x, 300-y);
   //setVelocity(vec3((400 - x), (300 - y), m_player->getPosition().z));
   }
}
