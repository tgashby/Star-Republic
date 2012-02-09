//
//  Explodeable.h
//  StarRepub
//
//  Created by Taggart Ashby on 2/9/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Explodeable_h
#define StarRepub_Explodeable_h

#include <string.h>
#include "Object3d.h"
#include "Mesh.h"
#include "Interfaces.h"

using namespace std;

enum ExplosionState 
{
   START = 0,
   MIDDLE = 1,
   END = 2,
   DONE = 3
};

class Explodeable : public Object3d {
public:
   Explodeable(string headName, vec3 position, Modules* modules);
   
   void explode();
   
   void tic(uint64_t dt);
   
private:
   Mesh* m_mesh;
   ExplosionState state;
   vec3 m_position;
   uint64_t stateTimer;
};

#endif
