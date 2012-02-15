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
   
   InitData();
}


GameEngine::~GameEngine() {
   list<IObject3d *>::iterator object = m_objects.begin();
   for (; object != m_objects.end(); ++object) {
      delete *object;
   }
   m_objects.clear();
   m_gameObjects.clear();

   //delete m_camera;
}

void GameEngine::InitData()
{
   m_world = new Path("maps/course.wf", m_modules);
   
   m_camera = new Camera(m_world->getCurrentPointer(), m_world->getPreviousPointer());
   m_player = new Player("models/spaceship.obj", "textures/test3.bmp", 
			 m_modules, m_camera->getPosition(), 
			 m_camera->getForward(), m_camera->getUp());
   m_camera->setPlayer(m_player);

   m_enemyShip = new EnemyShip("models/enemy.obj", "textures/test3.bmp", 
			       m_modules, *m_player);
   m_reticle = new Reticle("models/reticle2.obj", "textures/test3.bmp", 
			 m_modules, m_player);
   
   //explosion = new Explodeable(m_player->getPosition(), m_modules);

   m_turretLocs = m_modules->resourceManager->
   readWorldData("maps/course.wf");

   createTurrets();
   
   m_currentPoint = m_world->getCurrentPointer();
   m_previousPoint = m_world->getPreviousPointer();
   
   m_player->setProgress(m_previousPoint->getPosition());
   m_player->setPosition(m_previousPoint->getPosition());
   m_player->setUp(m_previousPoint->getUp());
   m_player->setHeads(m_currentPoint->getPosition(), 
		      m_currentPoint->getUp(), m_previousPoint->getPosition(), 
		      m_previousPoint->getUp());
   m_player->calculateSide();
   
   m_enemyShip->setProgress(m_previousPoint->getPosition());
   m_enemyShip->setPosition(m_previousPoint->getPosition());
   m_enemyShip->setUp(m_previousPoint->getUp());
   m_enemyShip->setHeads(m_currentPoint->getPosition(), 
			 m_currentPoint->getUp(), m_previousPoint->getPosition(), 
			 m_previousPoint->getUp());
   m_enemyShip->calculateSide();
   
   m_modules->renderingEngine->setCamera(m_camera);

   m_modules->renderingEngine->addObject3d(m_player);
   m_modules->renderingEngine->addObject3d(m_reticle);
   m_modules->renderingEngine->addObject3d(m_enemyShip);
   //m_modules->renderingEngine->addObject3d(explosion);
   
   for (std::vector<Turret*>::const_iterator i = m_turrets.begin(); 
	i != m_turrets.end(); i++) 
   {
      m_modules->renderingEngine->addObject3d(*i);
   }
   
   m_objects.push_back(m_player);
   m_objects.push_back(m_reticle);
   m_objects.push_back(m_enemyShip);
   //m_objects.push_back(explosion);
   
   for (std::vector<Turret*>::const_iterator i = m_turrets.begin(); 
	i != m_turrets.end(); i++) 
   {
      m_objects.push_back(*i);
   }
   
   m_gameObjects.push_back(m_enemyShip);
   m_gameObjects.push_back(m_player);
   
   for (std::vector<Turret*>::const_iterator i = m_turrets.begin(); 
	i != m_turrets.end(); i++) 
   {
      m_gameObjects.push_back(*i);
   }

   m_enemyShip->setPosition(m_player->getPosition() + (m_player->getForward() * 30));
   
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
   
   // Update functions go here
   m_world->update(m_camera->getRef());
   m_currentPoint = m_world->getCurrentPointer();

   m_enemyShip->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());
   m_enemyShip->setPosition(m_player->getPosition() + (m_camera->getForward() * 300));
   m_enemyShip->tic(td);
   
   m_camera->checkPath(m_world->getCurrentPointer());
   m_camera->tic(td);

   m_player->tic(td, m_camera->getPosition(), m_camera->getUp(), m_camera->getForward());
   m_reticle->tic(td);
   
   //explosion->setPosition(m_player->getPosition());
   //explosion->tic(td);

   for (std::vector<Turret*>::iterator i = m_turrets.begin(); i != m_turrets.end(); i++) 
   {
      (*i)->tic(td);
      
      vec3 dirToPlayer = (*i)->getPosition() - m_player->getPosition();
      
      // Turret not currently firing, but I think it's because
      //  the player starts too close to the turret
      if ((*i)->isAlive() && dirToPlayer.Length() < 1000 && (*i)->shouldFire()) 
      {
         vec3 dirToPlayerNorm = dirToPlayer.Normalized();
         
         Bullet* bullet = 
            new Bullet("models/cube.obj", "textures/test5.bmp", 
                       m_modules, (*i)->getHeadPosition(), 
                       -dirToPlayerNorm, 
                       dirToPlayerNorm.Cross((*i)->getPosition()), *(*i));
            
         m_modules->renderingEngine->addObject3d(bullet);
         m_gameObjects.push_back(bullet);
         m_objects.push_back(bullet);
         m_bulletList.push_back(bullet);
      }
   }

   vec3 dirEnemyToPlayer = m_enemyShip->getPosition() - m_player->getPosition();
   if (dirEnemyToPlayer.Length() < 400 && m_enemyShip->shouldFire()) 
   {
      vec3 dirToPlayerNorm = dirEnemyToPlayer.Normalized();
      
      Bullet* bullet = 
         new Bullet("models/cube.obj", "textures/test5.bmp", 
                    m_modules, m_enemyShip->getLeftCannonPos(), 
                    m_enemyShip->getAimForward(), 
                    dirToPlayerNorm.Cross(m_enemyShip->getLeftCannonPos()), 
		    *m_enemyShip, Bullet::defaultTimeToLive, 0.2f);
         
      m_modules->renderingEngine->addObject3d(bullet);
      m_gameObjects.push_back(bullet);
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);

      bullet = 
         new Bullet("models/cube.obj", "textures/test5.bmp", 
                    m_modules, m_enemyShip->getRightCannonPos(), 
                    m_enemyShip->getAimForward(), 
                    dirToPlayerNorm.Cross(m_enemyShip->getRightCannonPos()), 
		    *m_enemyShip, Bullet::defaultTimeToLive, 0.2f);
         
      m_modules->renderingEngine->addObject3d(bullet);
      m_gameObjects.push_back(bullet);
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);
   }
   
   //Use Iterators!
   //for (int i = 0; i < m_bulletList.size(); i++) {
   for(std::vector<Bullet *>::iterator bulletIterator = m_bulletList.begin();
       bulletIterator != m_bulletList.end();
       bulletIterator++){ 
      (*bulletIterator)->tic(td);
      //Cull the bullet!
      if(!(*bulletIterator)->isAlive()){
         //m_bulletList.erase(bulletIterator);
      }
         
   }

   for (std::vector<Missile *>::iterator missileIterator = m_missileList.begin(); 
	missileIterator != m_missileList.end(); 
	missileIterator++) {
      (*missileIterator)->tic(td);
      //Cull the missile!
   }

   runCollisions();
}


void GameEngine::render() {
   m_modules->renderingEngine->render(m_objects);
}


// Take care of mouse and keyboard events
bool GameEngine::handleEvents()
{
   bool running = true;
   
   SDL_Event evt;
   uint64_t old = SDL_GetTicks();
   
   
   while (SDL_PollEvent(&evt))
   {
      uint64_t now = SDL_GetTicks();
      
      old = now;
      
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
   }
   
   if (key == SDLK_z)
   {
      Bullet *bullet = new Bullet("models/cube.obj", "textures/test4.bmp", 
                                  m_modules, m_player->getPosition() 
                                  + (m_player->getSide() * 8),
                                  m_player->getAimForward(), m_player->getAimUp(), 
                                  *m_player, Bullet::defaultTimeToLive, 1.0f);
      
      m_modules->renderingEngine->addObject3d(bullet);
      m_gameObjects.push_back(bullet);
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);
      
      bullet = new Bullet("models/cube.obj", "textures/test4.bmp", 
                          m_modules, m_player->getPosition() 
                          - (m_player->getSide() * 8),
                          m_player->getAimForward(), m_player->getAimUp(), 
                          *m_player, Bullet::defaultTimeToLive, 1.0f);
      
      
      
      m_modules->renderingEngine->addObject3d(bullet);
      m_gameObjects.push_back(bullet);
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);
      
      
      //m_bulletSound->play(0);
   }

   return running;
}

std::vector<GameObject*> GameEngine::acquireMissileTargets() {
  std::vector<GameObject*> temp;

  for (list<GameObject *>::iterator it = m_gameObjects.begin(); 
       it != m_gameObjects.end(); it++) {
    if (typeid(*it) != typeid(Bullet *) && typeid(*it) != typeid(Player *)) {
	if (((*it)->getPosition() - m_player->getPosition()).Length() < 1000) {
	  temp.push_back(*it);
	}
    }
  }

  return temp;
}

bool GameEngine::handleKeyUp(SDLKey key)
{
   bool running = true;
   std::vector<GameObject*> targets;
   
   if (key == SDLK_ESCAPE) 
   {
      running = false;
   }

   if (key == SDLK_x) {

     targets = acquireMissileTargets();

     //For each target
     for (int index = 0; index < targets.size(); index++) {
       Missile *missile = new Missile("models/cube.obj", "textures/test6.bmp",
				      m_modules, 
				      m_player->getPosition(), 
				      m_player->getAimForward(), 
				      m_player->getAimUp(),
				      m_player, 
				      targets.at(index));

       m_modules->renderingEngine->addObject3d(missile);
       m_missileList.push_back(missile);
       m_objects.push_back(missile);
       m_gameObjects.push_back(missile);
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
      
      for(std::list<GameObject *>::iterator otherGameObjectIterator 
	     = m_gameObjects.begin();
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
   for (std::vector< Vector3<float> >::iterator i = m_turretLocs->turrets.begin(); 
	i != m_turretLocs->turrets.end();)
   {
      // Loc, forward, up
      Turret* newTurret = new Turret(*m_player, "models/turrethead.obj", 
				     "textures/test3.bmp", "models/turretmiddle.obj", 
				     "textures/test3.bmp", "models/turretbase.obj", 
				     "textures/test3.bmp", m_modules);
      
      newTurret->setPosition(*i);
      ++i;
      newTurret->setForward(*i);
      ++i;
      newTurret->setUp(*i);
      ++i;
      
      m_turrets.push_back(newTurret);
   }
   
   vector<string>::iterator name = m_turretLocs->worldMeshes.begin();
   vector<vec3>::iterator vec = m_turretLocs->worldLocs.begin();
   while (name != m_turretLocs->worldMeshes.end()) {
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
   }
}
