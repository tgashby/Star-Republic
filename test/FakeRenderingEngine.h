//
//  FakeRenderingEngine.h
//  StarRepublic
//
//  Created by Taggart Ashby on 2/25/12.
//  Copyright (c) 2012 476 Proj. All rights reserved.
//

#ifndef StarRepublic_FakeRenderingEngine_h
#define StarRepublic_FakeRenderingEngine_h

#include "Interfaces.h"

class FakeRenderingEngine : public IRenderingEngine 
{
public:
   void render(list<IObject3d *> &objects) {}
   void setCamera(ICamera *camera) {}
   void addObject3d(IObject3d *obj) {}
   void removeObject3d(IObject3d *obj) {}
   void drawText(string text, ivec2 loc, ivec2 size) {}
   void clearScreen() {}
};


#endif
