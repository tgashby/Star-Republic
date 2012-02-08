#include "GameEngine.h"
#include "Object3d.h"
#include "Player.h"



GameEngine::GameEngine(Modules *modules) {
   m_modules = modules;
   m_objects = list<IObject3d *>(0);

   InitData();
}


GameEngine::~GameEngine() {
   list<IObject3d *>::iterator object = m_objects.begin();
   for (; object != m_objects.end(); ++object) {
      delete *object;
   }
   m_objects.clear();
   //delete m_camera;
}

void GameEngine::InitData()
{
   // just push a single object to the list and add to the RenderingEngine
   m_player = new Player("models/spaceship.obj", "textures/test3.bmp", 
			 m_modules);
   m_reticle = new Reticle("models/reticle2.obj", "textures/test3.bmp", 
			 m_modules, m_player);
   
   m_enemyShip = new EnemyShip("models/spaceship.obj", "textures/test3.bmp", m_modules);

   m_camera = new Camera(vec3(0, 0, 0));
   m_world = new World("maps/world.wf");
   m_turret = new Turret(*m_player, "models/turrethead.obj",
                         "textures/test3.bmp",
                         "models/turretmiddle.obj",
                         "textures/test3.bmp",
                         "models/turretbase.obj",
                         "textures/test3.bmp",
                         m_modules);
   m_turret->setPosition(vec3(-144,-1168,5063));

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
   
   m_objects.push_back(m_player);
   m_objects.push_back(m_reticle);
   m_objects.push_back(m_turret);
   m_objects.push_back(m_enemyShip);
   
   // add one canyon mesh for now.
   Object3d *canyon = new Object3d("models/canyon.obj", "textures/test3.bmp", m_modules);
   m_modules->renderingEngine->addObject3d(canyon);
   m_objects.push_back(canyon);
   
   
   m_player->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());
   m_enemyShip->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());
   
   initSound();
   m_bulletSound = loadSound("sound/arwingShot.ogg");
   m_music = loadMusic("sound/venom.mp3");
   
   m_music->play(1);
}

void GameEngine::tic(uint64_t td) {
   // Update functions go here
   m_world->update(m_player->getProgress());
   m_currentPoint = m_world->getCurrentPointer();
   m_player->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());
   m_player->tic(td);
   m_enemyShip->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());
   m_enemyShip->tic(td);
   
   m_reticle->tic(td);
   m_turret->tic(td);

   vec3 dirToPlayer = (m_turret->getPosition() - m_player->getPosition()).Normalized();
   
   Bullet* bullet = 
      new Bullet("models/cube.obj", "textures/test4.bmp", 
                 m_modules, m_turret->getHeadPosition(), -dirToPlayer, dirToPlayer.Cross(m_turret->getPosition()));
      
   m_modules->renderingEngine->addObject3d(bullet);
   m_objects.push_back(bullet);
   m_bulletList.push_back(bullet);
   
   for (int i = 0; i < m_bulletList.size(); i++) {
      m_bulletList[i]->tic(td);
   }
   
   m_camera->update(((m_player->getPosition() - m_player->getProgress()) / 2) + m_player->getProgress(), 
		    m_player->getForward(), 
		    m_player->getUp());
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
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);

      bullet = new Bullet("models/cube.obj", "textures/test4.bmp", 
			  m_modules, m_player->getPosition() 
			  - (m_player->getSide() * 8),
			  -m_player->getAimForward(), m_player->getAimUp());

      m_modules->renderingEngine->addObject3d(bullet);
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
