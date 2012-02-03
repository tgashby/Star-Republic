#include "GameEngine.h"
#include "Object3d.h"
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
