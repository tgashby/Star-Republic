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
   FakeGameEngine(PathPointData h, PathPointData t)
   {
      PathPoint head(h.loc, h.fwd, h.up, h.links);
      PathPoint tail(t.loc, t.fwd, t.up, t.links);
      
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
