//
//  Object2d.h
//  StarRepub
//
//  Created by Robert Crosby on 3/13/12.
//  Copyright (c) 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Object2d_h
#define StarRepub_Object2d_h

#define PLANE_MESH_FILE "models/plane.obj"

#include "Interfaces.h"

class Object2d : public IObject3d {
public:
   Object2d(string textureName, ivec2 loc, ivec2 size, Modules *modules);
   Object2d();
   virtual ~Object2d();
   list<IMesh *>* getMeshes();
   void setVisible(bool vis);
   bool viewCull(vector<vec4> *planes);
protected:
   list<IMesh *> m_meshList;
};

#endif
