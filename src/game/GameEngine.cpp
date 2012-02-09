#include "GameEngine.h"
#include "Object3d.h"
#include "Player.h"

#define VELOCITY_CONSTANT 2

GameEngine::GameEngine(Modules *modules) {
   m_modules = modules;
   m_objects = list<IObject3d *>(0);
   m_gameObjects = list<GameObject *>(0);

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
   
      cerr << "MY LITTLE SEGFAULT!\n";
   cerr << "MY LITTLE SEGFAULT!\n";
   m_world = new World("maps/world.wf", m_modules);
   cerr << "I USED TO WONDER WHAT POINTERS COULD BE!\n";
   WorldPoint* debugtemp = m_world->getCurrentPointer();
   cerr << debugtemp->getPosition().x << " " << debugtemp->getPosition().y << " " << debugtemp->getPosition().z << "\n";
   m_camera = new Camera(m_world->getCurrentPointer(), m_world->getPreviousPointer());
   cerr << "Not the camera\n";
// just push a single object to the list and add to the RenderingEngine
   m_player = new Player("models/spaceship.obj", "textures/test3.bmp", 
			 m_modules, m_camera->getPosition(), m_camera->getForward(), m_camera->getUp());
   m_enemyShip = new EnemyShip("models/enemy.obj", "textures/test3.bmp", m_modules, *m_player);
   m_reticle = new Reticle("models/reticle2.obj", "textures/test3.bmp", 
			 m_modules, m_player);
   m_turret = new Turret(*m_player, "models/turrethead.obj",
                         "textures/test3.bmp",
                         "models/turretmiddle.obj",
                         "textures/test3.bmp",
                         "models/turretbase.obj",
                         "textures/test3.bmp",
                         m_modules);
cerr << "BAHHH! I'm awful\n";
   
   explosion = new Explodeable(m_player->getPosition(), m_modules);
   
   m_turret->setPosition(vec3(-144,-1168,4563));
   m_turret->setForward(-m_player->getForward());

   m_currentPoint = m_world->getCurrentPointer();
   m_previousPoint = m_world->getPreviousPointer();
   
   m_player->setProgress(m_previousPoint->getPosition());
   m_player->setPosition(m_previousPoint->getPosition());
   m_player->setUp(m_previousPoint->getUp());
   m_player->setHeads(m_currentPoint->getPosition(), m_currentPoint->getUp(), m_previousPoint->getPosition(), m_previousPoint->getUp());
   m_player->calculateSide();
   
   m_enemyShip->setProgress(m_previousPoint->getPosition());
   m_enemyShip->setPosition(m_previousPoint->getPosition());
   m_enemyShip->setUp(m_previousPoint->getUp());
   m_enemyShip->setHeads(m_currentPoint->getPosition(), m_currentPoint->getUp(), m_previousPoint->getPosition(), m_previousPoint->getUp());
   m_enemyShip->calculateSide();
   
   m_modules->renderingEngine->setCamera(m_camera);

   m_modules->renderingEngine->addObject3d(m_player);
   m_modules->renderingEngine->addObject3d(m_reticle);
   m_modules->renderingEngine->addObject3d(m_turret);
   m_modules->renderingEngine->addObject3d(m_enemyShip);
   m_modules->renderingEngine->addObject3d(explosion);
   
   m_gameObjects.push_back(m_player);
   m_objects.push_back(m_player);
   m_objects.push_back(m_reticle);
   m_objects.push_back(m_turret);
   m_gameObjects.push_back(m_turret);
   m_objects.push_back(m_enemyShip);
   m_gameObjects.push_back(m_enemyShip);
   m_objects.push_back(explosion);
   
   // add one canyon mesh for now.
   Object3d *canyon = new Object3d("models/canyon.obj", "textures/test3.bmp", m_modules);
   m_modules->renderingEngine->addObject3d(canyon);
   m_objects.push_back(canyon);
   /*canyon = new Object3d("models/course1b.obj", "textures/test3.bmp", m_modules);
   m_modules->renderingEngine->addObject3d(canyon);
   m_objects.push_back(canyon);
   canyon = new Object3d("models/course1c.obj", "textures/test3.bmp", m_modules);
   m_modules->renderingEngine->addObject3d(canyon);
   m_objects.push_back(canyon);
   canyon = new Object3d("models/course1d.obj", "textures/test3.bmp", m_modules);
   m_modules->renderingEngine->addObject3d(canyon);
   m_objects.push_back(canyon);
   canyon = new Object3d("models/course1e.obj", "textures/test3.bmp", m_modules);
   m_modules->renderingEngine->addObject3d(canyon);
   m_objects.push_back(canyon);*/
   
   
   //m_player->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());
   m_enemyShip->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());
   
   initSound();
   m_bulletSound = loadSound("sound/arwingShot.ogg");
   m_music = loadMusic("sound/venom.mp3");
   
   m_music->play(1);
   
   // test loading a world
   /*
   WorldData *worldData = m_modules->resourceManager->readWorldData("maps/world2.wf");
   cout << "loaded " << worldData->path.size() << " path elements\n";
   cout << "loaded " << worldData->links.size() << " link elements\n";
   cout << "loaded " << worldData->turrets.size() << " turret elements\n";
   cout << "(" << worldData->path[0].x << ", " << worldData->path[0].y << ")\n"; 
   delete worldData;*/
}

void GameEngine::tic(uint64_t td) {
   // Update functions go here
   cerr << "THIS IS STARTING TO TIC ME OFF!\n";
  m_world->update(m_camera->getRef());
   cerr << "I BE A WORLDLY MATEY!\n";
   m_currentPoint = m_world->getCurrentPointer();
   //m_player->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());
   //m_player->tic(td);
   m_enemyShip->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());
   m_enemyShip->tic(td);
   
   cerr << "YAR! TIC ENEMIES OFF!\n";
   m_turret->tic(td);

   m_camera->checkPath(m_world->getCurrentPointer());
   m_camera->tic(td);

   m_player->tic(td, m_camera->getPosition(), m_camera->getUp(), m_camera->getForward());
   m_reticle->tic(td);
   
   explosion->setPosition(m_player->getPosition());
   explosion->tic(td);

   vec3 dirToPlayer = m_turret->getPosition() - m_player->getPosition();//(m_player->getPosition() + (m_player->getForward().Normalized() * VELOCITY_CONSTANT));
   
   // Turret not currently firing, but I think it's because the player starts too close to the turret
   if (m_turret->getForward().Dot(m_player->getForward()) < 0 && dirToPlayer.Length() < 500 && m_turret->shouldFire()) 
   {
      vec3 dirToPlayerNorm = dirToPlayer.Normalized();
      
      Bullet* bullet = 
         new Bullet("models/cube.obj", "textures/test4.bmp", 
                    m_modules, m_turret->getHeadPosition(), 
                    -dirToPlayerNorm, 
                    dirToPlayerNorm.Cross(m_turret->getPosition()));
         
      m_modules->renderingEngine->addObject3d(bullet);
      m_gameObjects.push_back(bullet);
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);
   }

   vec3 dirEnemyToPlayer = m_enemyShip->getPosition() - m_player->getPosition();
   if (dirEnemyToPlayer.Length() < 400 && m_enemyShip->shouldFire()) 
   {
      vec3 dirToPlayerNorm = dirToPlayer.Normalized();
      
      Bullet* bullet = 
         new Bullet("models/cube.obj", "textures/test4.bmp", 
                    m_modules, m_enemyShip->getLeftCannonPos(), 
                    m_enemyShip->getAimForward(), 
                    dirToPlayerNorm.Cross(m_enemyShip->getLeftCannonPos()));
         
      m_modules->renderingEngine->addObject3d(bullet);
      m_gameObjects.push_back(bullet);
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);

      bullet = 
         new Bullet("models/cube.obj", "textures/test4.bmp", 
                    m_modules, m_enemyShip->getRightCannonPos(), 
                    m_enemyShip->getAimForward(), 
                    dirToPlayerNorm.Cross(m_enemyShip->getRightCannonPos()));
         
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
         //cerr << "Culling Bullet!\n";
         //m_bulletList.erase(bulletIterator);
      }
         
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

bool GameEngine::handleKeyUp(SDLKey key)
{
   bool running = true;
   
   if (key == SDLK_ESCAPE) 
   {
      running = false;
   }

   if (key == SDLK_z)
   {
      Bullet *bullet = new Bullet("models/cube.obj", "textures/test4.bmp", 
				  m_modules, m_player->getPosition() 
				  + (m_player->getSide() * 8),
			     -m_player->getAimForward(), m_player->getAimUp());
      
      m_modules->renderingEngine->addObject3d(bullet);
      m_gameObjects.push_back(bullet);
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);

      bullet = new Bullet("models/cube.obj", "textures/test4.bmp", 
			  m_modules, m_player->getPosition() 
			  - (m_player->getSide() * 8),
			  -m_player->getAimForward(), m_player->getAimUp());

      m_modules->renderingEngine->addObject3d(bullet);
      m_gameObjects.push_back(bullet);
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);
      
      m_bulletSound->play(0);
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
   for(std::list<GameObject *>::iterator gameObjectIterator=m_gameObjects.begin();
   gameObjectIterator != m_gameObjects.end();
   gameObjectIterator++){ 
      for(std::list<GameObject *>::iterator otherGameObjectIterator
               =m_gameObjects.begin();
         otherGameObjectIterator != m_gameObjects.end();
         otherGameObjectIterator++){ 
            if(gameObjectIterator !=otherGameObjectIterator){
               if((*gameObjectIterator)->collidesWith(*(*otherGameObjectIterator))){
                  //cerr << "A collision has happeneded\n";
                  (*gameObjectIterator)->doCollision(*(*otherGameObjectIterator));
               //I'm not sure if this is a good idea
               }
            }
         }
   }
     
}
