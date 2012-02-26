//
//  FakeGameEngine.h
//  StarRepublic
//
//  Created by Taggart Ashby on 2/25/12.
//  Copyright (c) 2012 476 Proj. All rights reserved.
//

#ifndef StarRepublic_FakeGameEngine_h
#define StarRepublic_FakeGameEngine_h

#include "Interfaces.h"
#include "Camera.h"

class FakeGameEngine : public IGameEngine
{
public:
   FakeGameEngine()
   {
      PathPoint head(vec3(0,0,0), vec3(1,0,0), vec3(0,0,1), ivec4(0,0,0,0));
      PathPoint tail(vec3(0,0,0), vec3(1,0,0), vec3(0,0,1), ivec4(0,0,0,0));
      
      m_camera = new Camera(&head, &tail);
   }
   
   ~FakeGameEngine() 
   {
      
   }
   
   void tic(uint64_t td) {}
   void render() {}
   bool handleEvents() { return false; }
   bool handleKeyUp(SDLKey key) { return false; }
   void handleMouseMotion(Uint16 x, Uint16 y) {}
   ICamera& getCamera() { return *m_camera; }
   
   Camera* m_camera;
};

#endif
