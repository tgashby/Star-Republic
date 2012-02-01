#include "ApplicationEngine.h"
#include <iostream>

ApplicationEngine::ApplicationEngine(IResourceManager* resourceManager, IRenderingEngine* renderingEngine) {
   m_resourceManager = resourceManager;
   m_renderingEngine = renderingEngine;
   
   m_camera = new Camera(vec3(0, 0, 0));
   m_renderingEngine->setCamera(m_camera);
   
   upkey = downkey = leftkey = rightkey = false;
   mouseLeft = mouseMiddle = mouseRight = false;
   
   m_objects = list<IObject3d *>(0);
   
   //Object3d *newObject = new Object3d("cube.obj", "test3.bmp");
   Object3d *newObject = new Object3d("turret.obj", "test3.bmp");
   m_renderingEngine->addObject3d(newObject);
   m_objects.push_back(newObject);
}


ApplicationEngine::~ApplicationEngine() {
   list<IObject3d *>::iterator object = m_objects.begin();
   for (; object != m_objects.end(); ++object) {
      delete *object;
   }
   m_objects.clear();
   delete m_resourceManager;
   delete m_renderingEngine;
   delete m_camera;
}

void ApplicationEngine::handleKeyDown(const char *k) {
   switch (*k) {
      case 'w':
      case 'W':
         upkey = true;
         break;
      case 's':
      case 'S': 
         downkey = true;
         break;
      case 'a': 
      case 'A':
         leftkey = true;
         break;
      case 'd':
      case 'D': 
         rightkey = true;
         break;
   }
}

void ApplicationEngine::handleKeyUp(const char *k) {
   switch (*k) {
      case 'w':
      case 'W':
         upkey = false;
         break;
      case 's':
      case 'S': 
         downkey = false;
         break;
      case 'a': 
      case 'A':
         leftkey = false;
         break;
      case 'd':
      case 'D': 
         rightkey = false;
         break;
   }
}

void ApplicationEngine::handleMouseDown(int button, int mouseX, int mouseY) {
   if (button == 1) {
      m_lastMouseLoc = ivec2(mouseX, mouseY);
      mouseLeft = true;
   }
}

void ApplicationEngine::handleMouseMove(int mouseX, int mouseY) {
   if (mouseLeft) {
      m_camera->rotLocal(-mouseY*MOUSE_MOVE_SENSITIVITY, mouseX*MOUSE_MOVE_SENSITIVITY);
   }
}

void ApplicationEngine::handleMouseUp(int button, int mouseX, int mouseY) {
   if (button == 1) {
      mouseLeft = false;
   }
}

void ApplicationEngine::handelMouseWheel(int dir) {
   m_camera->moveInOut(dir * -MOUSE_WHEEL_SENSITIVITY);
}

void ApplicationEngine::tic(unsigned int td) {
   /*
   // move camera
   vec3 cMove(0, 0, 0);
   if (upkey)
      cMove.z = 1;
   if (downkey)
      cMove.z = -1;
   if (leftkey)
      cMove.x = -1;
   if (rightkey)
      cMove.x = 1;
   cMove = cMove * (td / 100.0);
   m_camera->moveLocal(cMove);
   */
}

void ApplicationEngine::render() {
   m_renderingEngine->render(m_objects);
}

