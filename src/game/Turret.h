//
//  Turret.h
//  StarRepub
//
//  Created by Taggart Ashby on 2/3/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Turret_h
#define StarRepub_Turret_h

#include "../engine/Interfaces.h"
#include "../engine/Object3d.h"
#include "../engine/Mesh.h"

class Turret : public Object3d {
public:
   Turret(string headName, string headTexture, string midName, string midTexture, string footName, string footTexture, Modules *modules);
   ~Turret();
private:
   int health;
   Mesh *m_headMesh;
   Mesh *m_midMesh;
   Mesh *m_footMesh;
};

#endif
