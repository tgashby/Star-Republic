//
//  Missle.h
//  StarRepub
//
//  Created by Chad Brantley on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepub_Missile_h
#define StarRepub_Missile_h

//the time it takes for the missile to reach its destination
#define _MISSILE_REACH_DEST_TIME 1000
#define _MISSILE_ARC_HEIGHT 300
#define _MISSILE_RADIUS .4f

#include "GameObject.h"
#include "Interfaces.h"
#include "Mesh.h"
#include "../engine/Object3d.h"

class Missile : public Object3d, public GameObject {
 public:

   Missile(string fileName, string textureName, Modules *modules,
	   vec3 pos, vec3 forw, vec3 up,
	   GameObject* parent, GameObject* target);
   ~Missile();

   void tic(uint64_t time);
   vec3 getPosition();
   vec3 getForward();
   vec3 getUp();

 private:
   void doCollision(GameObject &other);

   Mesh* m_mesh;
   float m_lifetime;
   vec3 m_archVecPos;
   vec3 m_aimPos;

   GameObject* m_parent;
   GameObject* m_target;
   
   vec3 m_origPos;
};

#endif
