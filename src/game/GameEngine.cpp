#include "GameEngine.h"
#include "../engine/Object3d.h"
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

   m_camera = new Camera(vec3(0, 0, 0));
   m_world = new World("maps/testWorld4.wf");
   m_turret = new Turret("models/cube.obj",
                         "textures/test3.bmp",
                         "models/cube.obj",
                         "textures/test3.bmp",
                         "models/spaceship.obj",
                         "textures/test3.bmp",
                         m_modules);

   m_currentPoint = m_world->getCurrentPointer();
   m_previousPoint = m_world->getPreviousPointer();
   m_player->setProgress(m_previousPoint->getPosition());
   m_player->setPosition(m_previousPoint->getPosition());
   m_player->setUp(m_previousPoint->getUp());
   m_player->setHeads(m_currentPoint->getPosition(), m_currentPoint->getUp(),
		      m_previousPoint->getPosition(), m_previousPoint->getUp());
   m_player->calculateSide();
   m_modules->renderingEngine->setCamera(m_camera);

   m_modules->renderingEngine->addObject3d(m_player);
   m_modules->renderingEngine->addObject3d(m_turret);
   m_objects.push_back(m_player);
   m_objects.push_back(m_turret);
   m_player->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());

}

void GameEngine::tic(uint64_t td) {
   // Update functions go here
   Bullet* b;
   m_world->update(m_player->getProgress());
   m_currentPoint = m_world->getCurrentPointer();
   m_player->setBearing(m_currentPoint->getPosition(), m_currentPoint->getUp());
   m_player->tic(td);
   
   for (int i = 0; i < m_bulletList.size(); i++) {
      m_bulletList[i]->tic(td);
   }
   m_camera->update(m_player->getProgress(), 
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
      float dt = (now - old) / 1000.0f;
      
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
			     m_player->getForward(), m_player->getUp());
      
      m_modules->renderingEngine->addObject3d(bullet);
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);

      bullet = new Bullet("models/cube.obj", "textures/test4.bmp", 
			  m_modules, m_player->getPosition() 
			  - (m_player->getSide() * 8),
			  m_player->getForward(), m_player->getUp());

      m_modules->renderingEngine->addObject3d(bullet);
      m_objects.push_back(bullet);
      m_bulletList.push_back(bullet);
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
