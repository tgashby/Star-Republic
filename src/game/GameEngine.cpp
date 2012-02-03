#include "GameEngine.h"
#include "../engine/Object3d.h"
#include "Player.h"


GameEngine::GameEngine(Modules *modules) {
   m_modules = modules;
   m_objects = list<IObject3d *>(0);
   
   m_camera = new Camera(vec3(0, 0, 0));
   modules->renderingEngine->setCamera(m_camera);

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
   Object3d *newObject = new Player("models/spaceship.obj", "textures/test3.bmp", m_modules);
   m_modules->renderingEngine->addObject3d(newObject);
   m_objects.push_back(newObject);
}

void GameEngine::tic(unsigned int td) {
   // Update the state of the game.
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
   
   return running;
}

void GameEngine::handleMouseMotion(Uint16 x, Uint16 y)
{
   // Rotate player?
}
