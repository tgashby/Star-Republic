#include "GameEngine.h"
#include "Object3d.h"
#include "Player.h"
#include "SceneObject.h"

#define VELOCITY_CONSTANT 2

GameEngine::GameEngine(Modules *modules) {
   m_modules = modules;
   m_objects = list<IObject3d *>(0);
   m_gameObjects = list<GameObject *>(0);

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

   //INIT DATA not being called, only called when the menu is left.
   //InitData();
}


GameEngine::~GameEngine() {
// list<IObject3d *>::iterator object = m_objects.begin();
// for (; object != m_objects.end(); ++object) {
// delete *object;
// }
   m_objects.clear();
   m_gameObjects.clear();

   //delete m_camera;
}

void GameEngine::InitData()
{
   m_worldData = m_modules->resourceManager->readWorldData("maps/Course1.wf");
   
   m_path = new Path(m_worldData);
   //m_world = new Path("maps/course.wf", m_modules);
   
   m_camera = new Camera(m_path->getCurrentPointer(), m_path->getPreviousPointer());
   m_skybox = new SkyBox("models/box3.obj", "textures/box3.bmp", m_modules, m_camera->getPosition());
   m_player = new Player("models/spaceship.obj", "textures/spaceship_pp.bmp",
m_modules, m_camera->getPosition(),
m_camera->getForward(), m_camera->getUp());
   m_camera->setPlayer(m_player);

   m_reticle = new Reticle("models/reticle2.obj", "textures/test3.bmp", 
			 m_modules, m_player);

   createTurrets();
   createTerrain();
   
   m_currentPoint = m_path->getCurrentPointer();
   m_previousPoint = m_path->getPreviousPointer();
   
   m_player->setProgress(m_previousPoint->getPosition());
   m_player->setPosition(m_previousPoint->getPosition());
   m_player->setUp(m_previousPoint->getUp());
   m_player->setHeads(m_currentPoint->getPosition(),
m_currentPoint->getUp(), m_previousPoint->getPosition(),
m_previousPoint->getUp());
   m_player->calculateSide();
   
   m_modules->renderingEngine->setCamera(m_camera);

   m_modules->renderingEngine->addObject3d(m_player);
   m_modules->renderingEngine->addObject3d(m_skybox);
   m_modules->renderingEngine->addObject3d(m_reticle);
   //m_modules->renderingEngine->addObject3d(explosion);
   
   for (std::vector<Turret*>::const_iterator i = m_turrets.begin();
i != m_turrets.end(); i++)
   {
      m_modules->renderingEngine->addObject3d(*i);
   }
   
   m_objects.push_back(m_player);
   m_objects.push_back(m_skybox);
   m_objects.push_back(m_reticle);
   //m_objects.push_back(explosion);
   
   for (std::vector<Turret*>::const_iterator i = m_turrets.begin();
i != m_turrets.end(); i++)
   {
      m_objects.push_back(*i);
   }
   
   m_gameObjects.push_back(m_player);
   
   for (std::vector<Turret*>::const_iterator i = m_turrets.begin();
i != m_turrets.end(); i++)
   {
      m_gameObjects.push_back(*i);
   }

   
   initSound();
   m_bulletSound = loadSound("sound/weapon1.wav");
   m_missileSound = loadSound("sound/missileLaunch.wav");;
   m_music = loadMusic("sound/ambient1.wav");
   m_boostSound = loadSound("sound/boost.wav");
   addAsteroids();
   m_music->play(-1);
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
   m_currentPoint = m_path->getCurrentPointer();
   
   m_camera->checkPath(m_path->getCurrentPointer());
   m_camera->tic(td);

   m_player->tic(td, m_camera->getPosition(), m_camera->getUp(), m_camera->getForward());
   m_reticle->tic(td);

   m_skybox->tic(td, m_player->getPosition());
   
   //explosion->setPosition(m_player->getPosition());
   //explosion->tic(td);

   /*for (std::vector<GameObject *>::iterator j = m_enemies.begin(); j != m_enemies.end(); j++) {
     (*j)->tic(td);
   }*/

   for (std::vector<Turret*>::iterator i = m_turrets.begin(); i != m_turrets.end(); i++) 
   {
      (*i)->tic(td);
      
      vec3 dirToPlayer = (*i)->getPosition() - m_player->getPosition();
      
      // Turret not currently firing, but I think it's because
      // the player starts too close to the turret
      if ((*i)->isAlive() && dirToPlayer.Length() < 1500 && (*i)->shouldFire())
      {
         vec3 dirToPlayerNorm = dirToPlayer.Normalized();
         
         Bullet* bullet = 
            new Bullet("models/lance.obj", "textures/test5.bmp", 
                       m_modules, (*i)->getHeadPosition(), 
                       -dirToPlayerNorm, 
                       dirToPlayerNorm.Cross((*i)->getPosition()), *(*i));
            
         m_modules->renderingEngine->addObject3d(bullet);
         m_gameObjects.push_back(bullet);
         m_objects.push_back(bullet);
         m_bulletList.push_back(bullet);
      }
   }

   for (std::vector<EnemyShip *>::iterator j = m_enemyShips.begin();
          j != m_enemyShips.end(); j++) {
     (*j)->tic(td);

     vec3 dirEnemyToPlayer = (*j)->getPosition() - m_player->getPosition();
     if (dirEnemyToPlayer.Length() < 700 && (*j)->shouldFire())
     {
        vec3 dirToPlayerNorm = dirEnemyToPlayer.Normalized();
      
        Bullet* bullet = 
          new Bullet("models/lance.obj", "textures/test5.bmp", 
                    m_modules, (*j)->getLeftCannonPos(), 
                    (*j)->getAimForward(), 
                    dirToPlayerNorm.Cross((*j)->getLeftCannonPos()), 
		            *(*j), Bullet::defaultTimeToLive, 0.3f);
         
        m_modules->renderingEngine->addObject3d(bullet);
        m_gameObjects.push_back(bullet);
        m_objects.push_back(bullet);
        m_bulletList.push_back(bullet);
        bullet = 
          new Bullet("models/lance.obj", "textures/test5.bmp", 
                    m_modules, (*j)->getRightCannonPos(), 
                    (*j)->getAimForward(), 
                    dirToPlayerNorm.Cross((*j)->getRightCannonPos()), 
		            *(*j), Bullet::defaultTimeToLive, 0.3f);
         
        m_modules->renderingEngine->addObject3d(bullet);
        m_gameObjects.push_back(bullet);
        m_objects.push_back(bullet);
        m_bulletList.push_back(bullet);
     }
   }

   for (std::vector<EnemyGunship *>::iterator j = m_enemyGunners.begin();
              j != m_enemyGunners.end(); j++) {
     (*j)->tic(td);

     vec3 dirEnemyToPlayer = (*j)->getPosition() - m_player->getPosition();
     if (dirEnemyToPlayer.Length() < 1600 &&
        ((*j)->shouldFire1() || (*j)->shouldFire2()))
     {
        vec3 dirToPlayerNorm = dirEnemyToPlayer.Normalized();
      
        if ((*j)->shouldFire1())
        {
           Bullet* bullet = 
              new Bullet("models/lance.obj", "textures/test5.bmp", 
                      m_modules, (*j)->getLeftCannonPos(), 
                      (*j)->getAimForward(), 
                      dirToPlayerNorm.Cross((*j)->getLeftCannonPos()), 
		              *(*j), Bullet::defaultTimeToLive, 0.5f);
           
           m_modules->renderingEngine->addObject3d(bullet);
           m_gameObjects.push_back(bullet);
           m_objects.push_back(bullet);
           m_bulletList.push_back(bullet);
        }
        if ((*j)->shouldFire2())
        {
           Bullet* bullet = 
               new Bullet("models/lance.obj", "textures/test5.bmp", 
                      m_modules, (*j)->getRightCannonPos(), 
                      (*j)->getAimForward(), 
                      dirToPlayerNorm.Cross((*j)->getRightCannonPos()), 
		              *(*j), Bullet::defaultTimeToLive, 0.5f);
           
           m_modules->renderingEngine->addObject3d(bullet);
           m_gameObjects.push_back(bullet);
           m_objects.push_back(bullet);
           m_bulletList.push_back(bullet);
        }
      }
   }

   int i = 0;
   //Use Iterators!
   //for (int i = 0; i < m_bulletList.size(); i++) {
   for(std::vector<Bullet *>::iterator bulletIterator = m_bulletList.begin();
       bulletIterator != m_bulletList.end();
       bulletIterator++){
      (*bulletIterator)->tic(td);
      //Cull the bullet!
      i++;   
   }
   
   for (std::vector<Missile *>::iterator missileIterator = m_missileList.begin();
	missileIterator != m_missileList.end();
	missileIterator++) {
      (*missileIterator)->tic(td);
      //Cull the missile!
   }

   runCollisions();
   cullObjects();

   if(m_player->getAlive() == false) {
      m_stateManager->pushState(m_lose);
      //SHOULD PUT CODE HERE TO FREE MOST EVERYTHING IN THE GAME.
   }
   int deathCount = 0;
   for (std::vector<EnemyShip *>::iterator j = m_enemyShips.begin();
          j != m_enemyShips.end(); j++) {
      if ((*j)->isAlive() == false)
         deathCount++;
   }
   for (std::vector<EnemyGunship *>::iterator j = m_enemyGunners.begin();
          j != m_enemyGunners.end(); j++) {
      if ((*j)->isAlive() == false)
         deathCount++;
   }
   if (deathCount == 1) {
      m_stateManager->pushState(m_win);
      //SHOULD PUT CODE HERE TO FREE MOST EVERYTHING IN THE GAME AS WELL.
   }   
   }
}

void GameEngine::cullObjects() {
  /*for (std::vector<Bullet *>::iterator bulletIter = m_bulletList.begin(); 
       bulletIter != m_bulletList.end(); bulletIter++) {
    if (isCullable(*bulletIter)) {
      cullObject(*bulletIter);
    }
    }*/
  
  vector<GameObject *> toCull;

  for (std::vector<Missile *>::iterator missileIter = m_missileList.begin();
       missileIter != m_missileList.end(); missileIter++) {
    if (isCullable(*missileIter)) {
      toCull.push_back(*missileIter);
    }
  }

  for (vector<GameObject *>::iterator cullIter = toCull.begin();
       cullIter != toCull.end(); cullIter++) {
    cullObject(*cullIter);
  }
}

bool GameEngine::isCullable(GameObject* obj) {
  if (typeid(*obj) == typeid(Bullet)) {
    return true;
  }

  if (typeid(*obj) == typeid(Missile)) {
    return true;
  }

  return false;
}

void GameEngine::cullObject(GameObject* obj) {
  if (typeid(*obj) == typeid(Bullet)) {
    //remove(m_bulletList.begin(), m_bulletList.end(), obj);
    m_bulletList.erase(find(m_bulletList.begin(), m_bulletList.end(), obj));
  }
  
  if (typeid(*obj) == typeid(Missile)) {
    //remove(m_missileList.begin(), m_missileList.end(), obj);
    //m_missileList.resize(m_missileList.size() - 1);
    m_missileList.erase(find(m_missileList.begin(), m_missileList.end(), obj));
  }

  //remove(m_objects.begin(), m_objects.end(), (Object3d*) obj);
  //m_objects.resize(m_objects.size() - 1);
  m_objects.erase(find(m_objects.begin(), m_objects.end(), (Object3d*) obj));
  //remove(m_gameObjects.begin(), m_gameObjects.end(), obj);
  //m_
  m_gameObjects.erase(find(m_gameObjects.begin(), m_gameObjects.end(), obj));

  delete obj;
}

void GameEngine::render() {
   //Checks if the current state is the game state. This could be made more elegant.
   if (m_stateManager->getCurrentState() == m_game)
   {
      m_modules->renderingEngine->render(m_objects);
   }
   if (m_stateManager->getCurrentState() == m_menu)
   {
      m_modules->renderingEngine->clearScreen();
      m_modules->renderingEngine->drawText("STAR REPUBLIC", ivec2(-350,0), ivec2(800,100));
      m_modules->renderingEngine->drawText("Press Any Button To Begin", ivec2(-350, -100), ivec2(500,50));
   }
   if (m_stateManager->getCurrentState() == m_lose)
   {
      m_modules->renderingEngine->clearScreen();
      m_modules->renderingEngine->drawText("YOU LOSE", ivec2(-350,0), ivec2(800,100));
      m_modules->renderingEngine->drawText("Close The Window", ivec2(-350, -100), ivec2(500,50));
   }
   if (m_stateManager->getCurrentState() == m_win)
   {
      m_modules->renderingEngine->clearScreen();
      m_modules->renderingEngine->drawText("CONGRAGULATIONS", ivec2(-350,0), ivec2(800,100));
      m_modules->renderingEngine->drawText("You've Won!", ivec2(-350, -100), ivec2(500,50));
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

   for (int pntIndex = 1; pntIndex < m_path->getSize(); pntIndex+=1) {
      current = m_path->getAt(pntIndex);

      //ADD AN ASTEROID
      if (pntIndex % 2 == 0) {
	tempShip = new EnemyShip("models/enemyship.obj", "textures/enemyshiptexture.bmp", m_modules, *m_player);

	tempShip->setPosition(current.getPosition());
        tempShip->tic(0);
	m_modules->renderingEngine->addObject3d(tempShip);
	m_gameObjects.push_back(tempShip);
	m_objects.push_back(tempShip);
	//m_enemies.push_back(tempShip);
    m_enemyShips.push_back(tempShip);
      }
      else {
	tempGunner = new EnemyGunship("models/enemy2.obj", "models/enemy2turretbase.obj", "models/enemy2turrethead.obj", "textures/enemy2texture.bmp", "textures/enemy2turretbasetex.bmp", "textures/enemy2turretheadtex.bmp", m_modules, *m_player);
	tempGunner->setPosition(current.getPosition());
        tempGunner->tic(0);
	m_modules->renderingEngine->addObject3d(tempGunner);
	m_gameObjects.push_back(tempGunner);
	m_objects.push_back(tempGunner);
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
         m_boostSound->play(-1);
      }
      m_camera->setBoosting(true);
      m_reticle->setVisible(false);
   }
   
   if (key == SDLK_z)
   {
      if (!m_camera->isBoosting()) {
	 Bullet *bullet = new Bullet("models/lance.obj", "textures/test4.bmp", 
				     m_modules, m_player->getPosition() 
				     + (m_player->getSide() * 8),
				     m_player->getAimForward(), m_player->getAimUp(), 
				     *m_player, Bullet::defaultTimeToLive, 1.0f);
	 
	 m_modules->renderingEngine->addObject3d(bullet);
	 m_gameObjects.push_back(bullet);
	 m_objects.push_back(bullet);
	 m_bulletList.push_back(bullet);
	 
	 bullet = new Bullet("models/lance.obj", "textures/test4.bmp", 
			     m_modules, m_player->getPosition() 
			     - (m_player->getSide() * 8),
			     m_player->getAimForward(), m_player->getAimUp(), 
			     *m_player, Bullet::defaultTimeToLive, 1.0f);
      
      
      
m_modules->renderingEngine->addObject3d(bullet);
m_gameObjects.push_back(bullet);
m_objects.push_back(bullet);
m_bulletList.push_back(bullet);
      
      
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

  for (list<GameObject *>::iterator it = m_gameObjects.begin();
       it != m_gameObjects.end(); it++) {
     if (typeid(**it) != typeid(Bullet) && typeid(**it) != typeid(Player) && typeid(**it) != typeid(Missile)) {
	playerToObjVec = (*it)->getPosition() - m_player->getPosition();
	if (playerToObjVec.Length() > 500 && 
	    playerToObjVec.Length() < 4000 && 
	    angleBetween(m_player->getAimForward(), playerToObjVec) < 60.0f &&
       (*it)->isAlive()) {
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
   if (key == SDLK_ESCAPE || m_stateManager->getCurrentState() != m_game)
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
	    
	    Missile *missile = new Missile("models/missile1.obj", "textures/test6.bmp",
					   m_modules, 
					   bulletOrigin, 
					   m_player->getAimForward(), 
					   curveDir,
					   m_player, 
					   targets.at(index));
	    
	    m_modules->renderingEngine->addObject3d(missile);
	    m_missileList.push_back(missile);
	    m_objects.push_back(missile);
	    m_gameObjects.push_back(missile);
	    m_missileSound->play(0);
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
      m_boostSound->stop();
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
void GameEngine::runCollisions()
{
   for(std::list<GameObject *>::iterator gameObjectIterator = m_gameObjects.begin();
   gameObjectIterator != m_gameObjects.end();
   gameObjectIterator++)
   {
      
      for(std::list<GameObject *>::iterator otherGameObjectIterator= m_gameObjects.begin();
          otherGameObjectIterator != m_gameObjects.end();
          otherGameObjectIterator++)
      {
         if ((*gameObjectIterator)->isAlive() && (*otherGameObjectIterator)->isAlive())
         {
            if(gameObjectIterator != otherGameObjectIterator)
            {
               if((*gameObjectIterator)->collidesWith(*(*otherGameObjectIterator)))
               {
                  (*gameObjectIterator)->doCollision(*(*otherGameObjectIterator));
               }
            }
         }
      }
   }
     
}

void GameEngine::createTurrets()
{
   vector<PathPointData>::iterator point;
   vector<UnitData>::iterator unit;
   
   for (point = m_worldData->path.begin(); point != m_worldData->path.end(); ++point) {
      for (unit = point->units.begin(); unit != point->units.end(); ++unit) {
         if (unit->type == UNIT_TURRET) {
            Turret* newTurret = new Turret(*m_player, m_modules);
            //Turret* newTurret = new Turret(*m_player, "models/turrethead.obj", 
            //                               "textures/turretHeadTex.bmp", "models/turretMid.obj", 
            //                               "textures/turretMidTex.bmp", "models/turretbase.obj", 
            //                               "textures/turretBaseTex.bmp", m_modules);
            newTurret->setPosition(unit->loc);
            newTurret->setForward(unit->fwd);
            newTurret->setUp(unit->up);
            
            m_turrets.push_back(newTurret);
         }
      }
   }
   
   /*
for (std::vector< Vector3<float> >::iterator i = m_world->worldData->turrets.begin();
i != m_world->worldData->turrets.end(); i += 3)
{
// Loc, forward, up
Turret* newTurret = new Turret(*m_player, "models/turrethead.obj",
"textures/test3.bmp", "models/turretmiddle.obj",
"textures/test3.bmp", "models/turretbase.obj",
"textures/test3.bmp", m_modules);
newTurret->setPosition(*i);
newTurret->setForward(*(i + 1));
newTurret->setUp(*(i + 2));
m_turrets.push_back(newTurret);
}*/
}

void GameEngine::createTerrain()
{
   vector<PathPointData>::iterator point;
   vector<PropData>::iterator prop;
   
   for (point = m_worldData->path.begin(); point != m_worldData->path.end(); ++point) {
      for (prop = point->props.begin(); prop != point->props.end(); ++prop) {
         string fileName = "models/" + prop->name + ".obj";
         string texture = "textures/" + prop->name + ".bmp";
         SceneObject *obj = new SceneObject(fileName, texture, prop->loc, prop->fwd, prop->up, m_modules);
         m_modules->renderingEngine->addObject3d(obj);
         m_objects.push_back(obj);
      }
   }
   
   /*
vector<string>::iterator name = m_world->worldData->worldMeshes.begin();
vector<vec3>::iterator vec = m_world->worldData->worldLocs.begin();
while (name != m_world->worldData->worldMeshes.end()) {
vec3 pos = *vec;
++vec;
vec3 fwd = *vec;
++vec;
vec3 up = *vec;
++vec;
string file = "models/" + *name;
++name;
SceneObject *obj = new SceneObject(file, "textures/test3.bmp", pos, fwd, up, m_modules);
m_modules->renderingEngine->addObject3d(obj);
m_objects.push_back(obj);
}*/
}
