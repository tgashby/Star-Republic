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
#include "GameObject.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Mesh.h"

class Turret : public Object3d, public Enemy, public GameObject {
public:
   Turret(Player& player, string headName, string headTexture, string midName, string midTexture, string footName, string footTexture, Modules *modules);
   ~Turret();
   
   void tic(uint64_t time);
   void doCollision(GameObject & other);
   
   Vector3<float> getHeadPosition();
   
   bool shouldFire();
   bool isAlive();
   
private:
   Mesh *m_headMesh;
   Mesh *m_midMesh;
   Mesh *m_footMesh;
   
   bool firing;
   bool alive;
   uint64_t firingTimer;
   
   void collideWith(Bullet& bullet);
   void collideWith(Player& player);
   void collideWith(Enemy& enemy);
   
//   std::list<Bullet*> m_bullets;
};

#endif
