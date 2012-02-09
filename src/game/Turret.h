//
//  Turret.h
//  StarRepub
//
//  Created by Taggart Ashby on 2/3/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Turret_h
#define StarRepub_Turret_h

#include "Interfaces.h"
#include "Object3d.h"
#include "Player.h"
#include "Enemy.h"
#include "Mesh.h"

class Turret : public Object3d, public Enemy {
public:
   Turret(Player& player, string headName, string headTexture, string midName, string midTexture, string footName, string footTexture, Modules *modules);
   ~Turret();
   
   void tic(uint64_t time);
   void doCollision(GameObject & other);
   
private:
   int health;
   Mesh *m_headMesh;
   Mesh *m_midMesh;
   Mesh *m_footMesh;
};

#endif
