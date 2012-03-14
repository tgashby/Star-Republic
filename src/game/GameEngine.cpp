#include "GameEngine.h"
#include "Object3d.h"
#include "Object2d.h"
#include "Player.h"
#include "SceneObject.h"

#define VELOCITY_CONSTANT 2

GameEngine::GameEngine(Modules *modules) {
   m_modules = modules;
   
   gameOver = 0;
   m_stateManager = new StateManager();
   m_menu = new MenuState();
   m_game = new GameState();
   m_lose = new LoseState();
   m_win = new WinState();
   m_game->initialize();
   m_menu->initialize();
   m_lose->initialize();
   m_win->initialize();
   m_stateManager->pushState(m_game);
   m_stateManager->pushState(m_menu);
   
   m_menuImage = new Object2d("textures/mainmenu.bmp", ivec2(-300, -200), ivec2(600, 400), m_modules);
   m_modules->renderingEngine->addObject3d(m_menuImage);
   
   m_victoryImage = new Object2d("textures/victory.bmp", ivec2(-300, -200), ivec2(600, 400), m_modules);
   m_modules->renderingEngine->addObject3d(m_victoryImage);
   
   m_gameOverImage = new Object2d("textures/gameover.bmp", ivec2(-300, -200), ivec2(600, 400), m_modules);
   m_modules->renderingEngine->addObject3d(m_gameOverImage);
   
   m_modules->renderingEngine->setCamera(new Camera());
   
   /*
   m_test1 = new Object2d("textures/test7.bmp", ivec2(-16, -16), ivec2(64, 64), m_modules);
   m_modules->renderingEngine->addObject3d(m_test1);
   
   m_test2 = new Object2d("textures/test5.bmp", ivec2(-48, -48), ivec2(64, 64), m_modules);
   m_modules->renderingEngine->addObject3d(m_test2);*/
   
   //INIT DATA not being called, only called when the menu is left.
   //InitData();
}

GameEngine::~GameEngine() {
   // list<IObject3d *>::iterator object = m_objects.begin();
   // for (; object != m_objects.end(); ++object) {
   // delete *object;
   // }
   //   m_objects.clear();
   //   m_gameObjects.clear();
   
   //delete m_camera;
}

void GameEngine::InitData()
{
   m_worldData = m_modules->resourceManager->readWorldData("maps/Course1.wf");
   
   m_path = new Path(m_worldData);
   
   m_main = new Menu("models/menupanel.obj", "textures/mainmenu.bmp", m_modules);
   m_victory = new Menu("models/menupanel.obj", "textures/victory.bmp", m_modules);
   m_gameover = new Menu("models/menupanel.obj", "textures/gameover.bmp", m_modules);

   m_camera = new Camera(m_path->getCurrentPointer(), m_path->getPreviousPointer());
   m_skybox = new SkyBox("models/box3.obj", "textures/box3.bmp", m_modules, m_camera->getPosition());
   m_player = new Player("models/spaceship.obj", "textures/spaceship2.bmp",
                         m_modules, m_camera->getPosition(),
                         m_camera->getForward(), m_camera->getUp());
   
   m_camera->setPlayer(m_player);
   
   m_worldGrid = new WorldGrid(*m_path, *m_worldData, m_modules, m_player, &m_bulletList, &m_missileList);
   
   m_reticle = new Reticle("models/reticle2.obj", "textures/white.bmp", 
                           m_modules, m_player);
   
   //   m_enemyShip = new EnemyShip("models/enemy.obj", "textures/test3.bmp", 
   //			       m_modules, m_player);
   /*m_enemyGunner = new EnemyGunship("models/enemy2.obj", "models/enemy2turretbase.obj",
    "models/enemy2turrethead.obj", "textures/test3.bmp", m_modules, *m_player);*/
   
   m_player->setProgress(m_path->getPreviousPointer()->getPosition());
   m_player->setPosition(m_path->getPreviousPointer()->getPosition());
   m_player->setUp(m_path->getPreviousPointer()->getUp());
   m_player->setHeads(m_path->getCurrentPointer()->getPosition(), 
                      m_path->getCurrentPointer()->getUp(), m_path->getPreviousPointer()->getPosition(), 
                      m_path->getPreviousPointer()->getUp());
   m_player->calculateSide();
   
   m_modules->renderingEngine->setCamera(m_camera);
   
   m_modules->renderingEngine->addObject3d(m_player);
   m_modules->renderingEngine->addObject3d(m_skybox);
   m_modules->renderingEngine->addObject3d(m_reticle);
   m_modules->renderingEngine->addObject3d(m_main);
   //m_modules->renderingEngine->addObject3d(explosion);
   
   //   m_worldGrid->placeInGrid(m_enemyShip, m_enemyShip);
   //   m_worldGrid->placeInGrid(m_enemyGunner, m_enemyGunner);
   //m_objects.push_back(explosion);
   
   m_modules->soundManager->playBackgroundSound(Idle); 
   addAsteroids();
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
       if (!m_player->isAlive()) {
       cout << "YOU LOSE!\n";
       exit(0);
       }*/
      
      // Update functions go here
      m_path->update(m_camera->getRef(), m_player->getPosition());
      
      m_camera->checkPath(m_path->getCurrentPointer());
      m_camera->tic(td);
      
      m_player->tic(td, m_camera->getPosition(), m_camera->getUp(), m_camera->getForward());
      m_reticle->tic(td);
      
      m_skybox->tic(td, m_player->getPosition());
      
      for (vector<Missile*>::iterator i = m_missileList.begin(); i != m_missileList.end(); i++) 
      {
         (*i)->tic(td);
      }
      
      m_worldGrid->tic(td);
      
      m_worldGrid->checkCollisions();
      
      cullObjects();
      
      if(m_player->getAlive() == false) {
         m_stateManager->pushState(m_lose);
         //SHOULD PUT CODE HERE TO FREE MOST EVERYTHING IN THE GAME.
      }
      
      bool allObjsDead = true;
      for (vector<Objective*>::iterator objIter = m_objectives.begin();
           objIter != m_objectives.end(); objIter++) {
         if ((*objIter)->isAlive()) {
            allObjsDead = false;
         }
      }
      
      if (allObjsDead) {
         m_stateManager->pushState(m_win);
      }
   }
}

void GameEngine::cullObjects() {
   /**/
   
   vector<GameObject *> toCull;
   vector<Object3d *> toCullD;
   
   for (std::vector<Missile *>::iterator missileIter = m_missileList.begin();
        missileIter != m_missileList.end(); missileIter++) {
      if (isCullable(*missileIter)) {
         toCull.push_back(*missileIter);
         toCullD.push_back(*missileIter);
      }
   }
   
   for (std::vector<Bullet *>::iterator bulletIter = m_bulletList.begin(); 
        bulletIter != m_bulletList.end(); bulletIter++) {
      if (isCullable(*bulletIter)) {
         toCull.push_back(*bulletIter);
         toCullD.push_back(*bulletIter);
      }
   }
   
   std::vector<Object3d *>::iterator cullIterD = toCullD.begin();
   for (std::vector<GameObject *>::iterator cullIter = toCull.begin();
        cullIter != toCull.end() && cullIterD != toCullD.end(); cullIter++, cullIterD++) {
      cullObject(*cullIter, *cullIterD);
   }
}

bool GameEngine::isCullable(GameObject* obj) {
   if (typeid(*obj) == typeid(Bullet)) {
      if ((m_player->getPosition() - obj->getPosition()).Length() > 2000) {
         return true;
      }
   }
   
   if (typeid(*obj) == typeid(Missile)) {
      if (((Missile *)obj)->getTotalTime() > 4000) {
         return true;
      }
   }
   
   return false;
}

void GameEngine::cullObject(GameObject* obj, Object3d* second) {
   // cerr << "In here\n";
   if (typeid(*obj) == typeid(Bullet)) {
      //remove(m_bulletList.begin(), m_bulletList.end(), obj);
      //cerr << "Before : " <<m_bulletList.size() << "\n";
      m_bulletList.erase(find(m_bulletList.begin(), m_bulletList.end(), obj));
      //cerr << "After : " <<m_bulletList.size() << "\n";
      //m_modules->renderingEngine->removeObject3d((Bullet*)obj);
   }
   
   if (typeid(*obj) == typeid(Missile)) {
      //remove(m_missileList.begin(), m_missileList.end(), obj);
      //m_missileList.resize(m_missileList.size() - 1);
      m_missileList.erase(find(m_missileList.begin(), m_missileList.end(), obj));
      //remove(find(m_missileList.begin(), m_missileList.end(), obj));
   }
   
   //remove(m_objects.begin(), m_objects.end(), (Object3d*) obj);
   //m_objects.resize(m_objects.size() - 1);
   
   /*for (list<IObject3d *>::iterator objIter = m_objects.begin();
	 objIter != m_objects.end(); objIter++) {
    if (((GameObject *)(*objIter))->getPosition() == obj->getPosition()) {
    assert(false);
    m_objects.erase(objIter);
    break;
    }
    }*/
   /*ARE YOU RRRRREAAAAADY TO SUDOCODE
    FIRST! YOU ADD A NEW LAST OBJECT3D  TO THE LIST THAT HAS NOTHING OF VALUE
    THEN! YOU RUN FIND ON THE LIST FOR THE THINGY!
    SECOND! YOU COMPARE THE RESULT OF THE FIND WITH THE LAST VALUE IN THE THINGY!
    FINALLY! IF THEY ARE EQUAL, NOTHING OF VALUE WAS FOUND!
    */
   //  IObject3d* dummy = NULL;
   //  std::_List_iterator<IObject3d *> temp;
   //  m_objects.push_back(dummy);
   //  temp = find(m_objects.begin(), m_objects.end(), second);
   //  assert(temp != m_objects.end());
   //  m_objects.pop_back();
   //  m_objects.erase(find(m_objects.begin(), m_objects.end(), second));
   
   //m_objects.erase(myfind(m_objects.begin(), m_objects.end(), obj));
   
   
   //remove(m_gameObjects.begin(), m_gameObjects.end(), obj);
   //m_
   //  m_gameObjects.erase(find(m_gameObjects.begin(), m_gameObjects.end(), obj));
   
   delete obj;
   //cerr << "deleted the object\n";
}

void GameEngine::render() {
   list<IObject3d*> objs3d = list<IObject3d*>(0);
   list<IObject3d*> objs2d = list<IObject3d*>(0);
   
   //Checks if the current state is the game state. This could be made more elegant.
   if (m_stateManager->getCurrentState() == m_game)
   {
      objs3d = m_worldGrid->getDrawableObjects();
      
      //      std::cerr << "List Size: " << objs.size() << "\n";
      
      objs3d.push_back(m_skybox);
      objs3d.push_back(m_reticle);
      objs3d.push_back(m_player);
      m_modules->renderingEngine->render(objs3d, objs2d);
   }
   if (m_stateManager->getCurrentState() == m_menu)
   {
      /*
      m_modules->renderingEngine->clearScreen();
      m_modules->renderingEngine->drawText("STAR REPUBLIC", ivec2(-350,0), ivec2(800,100));
      m_modules->renderingEngine->drawText("Press Any Button To Begin", ivec2(-350, -100), ivec2(500,50));
       */
      objs2d.push_back(m_menuImage);
      m_modules->renderingEngine->render(objs3d, objs2d);

      /*list<IObject3d*> objs;
      objs.push_back(m_main);
      m_modules->renderingEngine->render(objs);
      m_modules->renderingEngine->drawMesh(m_main->m_mesh, m_main->modelMtx);*/
   }
   if (m_stateManager->getCurrentState() == m_lose)
   {
      /*
      m_modules->renderingEngine->clearScreen();
      m_modules->renderingEngine->drawText("YOU LOSE", ivec2(-350,0), ivec2(800,100));
      m_modules->renderingEngine->drawText("Close The Window", ivec2(-350, -100), ivec2(500,50));*/
      objs2d.push_back(m_gameOverImage);
      m_modules->renderingEngine->render(objs3d, objs2d);
   }
   if (m_stateManager->getCurrentState() == m_win)
   {
      /*
      m_modules->renderingEngine->clearScreen();
      m_modules->renderingEngine->drawText("CONGRAGULATIONS", ivec2(-350,0), ivec2(800,100));
      m_modules->renderingEngine->drawText("You've Won", ivec2(-350, -100), ivec2(500,50));*/
      objs2d.push_back(m_victoryImage);
      m_modules->renderingEngine->render(objs3d, objs2d);
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
      // if (evt.type == SDL_MOUSEBUTTONUP)
      // {
      // mouse_click(evt.button.button);
      // }
   }
   
   return running;
}

void GameEngine::addAsteroids() {
   EnemyShip* tempShip;
   EnemyGunship* tempGunner;
   PathPoint current(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0), vec3(0,0,0));
   
   //TEMPORARY!!!
   Objective* objective = new Objective("models/sphere.obj", 
                                        "textures/test6.bmp", m_modules, 
                                        (vec3(.0868337, 0.995747, -0.0307775) * 2000.0f) 
                                        + vec3(-266.174, 1759.54, -204.056),
                                        vec3 (0, 0, 1), vec3(0, 1, 0));
   m_modules->renderingEngine->addObject3d(objective);
   m_objectives.push_back(objective);
   m_path->addToQuadrants(objective->getPosition(), objective, objective);
   
   objective = new Objective("models/sphere.obj", 
                             "textures/test6.bmp", m_modules, 
                             (vec3(0.642882, -0.695466, -0.320984) * 1300.0f) 
                             + vec3(1373.04, -1224.47, -6905.99),
                             vec3 (0, 0, 1), vec3(0, 1, 0));
   m_modules->renderingEngine->addObject3d(objective);
   m_objectives.push_back(objective);
   m_path->addToQuadrants(objective->getPosition(), objective, objective);
   
   
   for (int pntIndex = 1; pntIndex < m_path->getSize(); pntIndex+=1) {
      current = m_path->getAt(pntIndex);
      
      //ADD AN ASTEROID
      if (pntIndex % 2 == 0) {
         tempShip = new EnemyShip("models/enemyship.obj", "models/enemysmaller.obj", "textures/enemyshiptexture.bmp", m_modules, m_player);
         
         tempShip->setSpawnPosition(current.getPosition());
         tempShip->tic(0);
         
         m_path->addToQuadrants(current.getPosition(), tempShip, tempShip);
         m_modules->renderingEngine->addObject3d(tempShip);
         
         //m_enemies.push_back(tempShip);
         m_enemyShips.push_back(tempShip);
      }
      else {
         tempGunner = new EnemyGunship("models/enemy2.obj", "models/enemy2smaller.obj", "models/enemy2turretbase.obj", "models/enemy2turrethead.obj", "textures/enemy2texture.bmp", "textures/enemy2turretbasetex.bmp", "textures/enemy2turretheadtex.bmp", m_modules, m_player);
         tempGunner->setSpawnPosition(current.getPosition());
         tempGunner->tic(0);
         
         m_path->addToQuadrants(current.getPosition(), tempGunner, tempGunner);
         m_modules->renderingEngine->addObject3d(tempGunner);
         
         //m_enemies.push_back(tempGunner);
         m_enemyGunners.push_back(tempGunner);
      }
   }
}

bool GameEngine::handleKeyDown(SDLKey key) {
   bool running = true;
   
   //Checks to see whether the current state is the menu, and pops the state if so. Will be revised later.
   if (m_stateManager->getCurrentState() == m_lose || m_stateManager->getCurrentState() == m_win)
   {
      return running;
   }
   if (m_stateManager->getCurrentState() == m_menu)
   {
      m_stateManager->popState();
      InitData();
      return running;
   }
   if (key == SDLK_SPACE) {
      if(!m_camera->isBoosting()){
         m_modules->soundManager->playBackgroundSound(Boost); 
      }
      m_camera->setBoosting(true);
      m_reticle->setVisible(false);
   }
   
   if (key ==  SDLK_LSHIFT)
      m_player->toggleMagnetic();
   
   if (key == SDLK_z)
   {
      if (!m_camera->isBoosting()) {
         Bullet *bullet = new Bullet("models/lance.obj", "textures/test4.bmp", 
                                     m_modules, m_player->getPosition() 
                                     + (m_player->getSide() * 8),
                                     m_player->getMagneticForward(), 
                                     m_player->getAimUp(), 
                                     *m_player, Bullet::defaultTimeToLive, 1.0f);
         
         //m_path->addToQuadrants(bullet->getPosition(), bullet, bullet);
         m_bulletList.push_back(bullet);
         m_modules->renderingEngine->addObject3d(bullet);
         
         bullet = new Bullet("models/lance.obj", "textures/test4.bmp", 
                             m_modules, m_player->getPosition() 
                             - (m_player->getSide() * 8),
                             m_player->getMagneticForward(), m_player->getAimUp(), 
                             *m_player, Bullet::defaultTimeToLive, 1.0f);
         
         m_bulletList.push_back(bullet);
         //m_path->addToQuadrants(bullet->getPosition(), bullet, bullet);
         m_modules->renderingEngine->addObject3d(bullet);
         
         
         
         m_modules->soundManager->playSound(PlayerGun); 
         //m_bulletSound->play(0);
      }
   }
   
   // Shotgun!
   if (key == SDLK_c) 
   {
      if (!m_camera->isBoosting()) {
         
         for (int currBullet = 0; currBullet < 6; currBullet++) 
         {
            int multiplier = (currBullet % 2) == 0 ? -1 : 1;
            
            vec3 magFwd = m_player->getMagneticForward();
            
            vec3 bFwd = vec3(magFwd.x * ((float)rand() / RAND_MAX + 2),
                             magFwd.y * ((float)rand() / RAND_MAX + 2),
                             magFwd.z * ((float)rand() / RAND_MAX + 2));
            
            Bullet *bullet = new Bullet("models/lance.obj", "textures/test4.bmp", 
                                        m_modules, 
                                        m_player->getPosition() + ((m_player->getSide() * 8) * multiplier),
                                        bFwd, 
                                        m_player->getAimUp(), 
                                        *m_player, Bullet::defaultTimeToLive, 1.0f);
            
            //m_path->addToQuadrants(bullet->getPosition(), bullet, bullet);
            m_bulletList.push_back(bullet);
            m_modules->renderingEngine->addObject3d(bullet);
         }
         
         m_modules->soundManager->playSound(PlayerGun); 
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
      if (typeid(**it) != typeid(Bullet) && typeid(**it) != typeid(Player) 
          && typeid(**it) != typeid(Missile) && (*it)->isAlive()) {
         playerToObjVec = (*it)->getPosition() - m_player->getPosition();
         if (playerToObjVec.Length() > 500 && 
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
   if (key == SDLK_ESCAPE || m_stateManager->getCurrentState() == m_lose || m_stateManager->getCurrentState() == m_win)
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
            
            Missile *missile = new Missile("models/missile1.obj", 
                                           "textures/missileTex.bmp", 
                                           m_modules, bulletOrigin, 
                                           m_player->getAimForward(), curveDir, 
                                           m_player, targets.at(index));            
            
            m_missileList.push_back(missile);
            m_modules->renderingEngine->addObject3d(missile);
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
      m_modules->soundManager->stopBackgroundSound(Boost); 
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

/**
 * This runs the collision functions on all objects upon which collide with the
 * given object.
 */
//void GameEngine::runCollisions()
//{
//   for(std::list<GameObject *>::iterator gameObjectIterator = m_gameObjects.begin();
//   gameObjectIterator != m_gameObjects.end();
//   gameObjectIterator++)
//   {
//      
//      for(std::list<GameObject *>::iterator otherGameObjectIterator= m_gameObjects.begin();
//          otherGameObjectIterator != m_gameObjects.end();
//          otherGameObjectIterator++)
//      {
//         if ((*gameObjectIterator)->isAlive() && (*otherGameObjectIterator)->isAlive())
//         {
//            if(gameObjectIterator != otherGameObjectIterator)
//            {
//               if((*gameObjectIterator)->collidesWith(*(*otherGameObjectIterator)))
//               {
//                  (*gameObjectIterator)->doCollision(*(*otherGameObjectIterator));
//               }
//            }
//         }
//      }
//   }
//     
//}
