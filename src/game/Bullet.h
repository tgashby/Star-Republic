//
//  Bullet.h
//  StarRepub
//
//  Created by Chad Brantley on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#pragma once

#include "../engine/Interfaces.h"
#include "../engine/Camera.h"
#include "../engine/Object3d.h"
#include "../engine/Mesh.h"
#include "GameObject.h"

class Bullet : public Object3d, public GameObject {
public:
   //tine to live in milliseconds
   static const uint64_t defaultTimeToLive=9000;
   //Default bullet speed
   static const float defaultSpeed=0.4f;
   //Default collision radius
   static const float defaultBulletRadius = 0.5f;

   Bullet(string fileName, string textureName, Modules *modules, 
    Vector3<float> pos, Vector3<float> forw, Vector3<float> up,
          GameObject& parent,
    const uint64_t timeToLive = Bullet::defaultTimeToLive, 
          const float speed = Bullet::defaultSpeed); 
   ~Bullet();

   void tic(uint64_t time);
   Vector3<float> getPosition();
   Vector3<float> getForward();
   Vector3<float> getUp();
   void calculateSide();
   void doCollision(GameObject & other);
   
   GameObject& getParent() { return m_parent; }

 private:

   Mesh* m_mesh;
   uint64_t m_lifetime; 
   uint64_t m_timeToLive;
   float m_speed;
   
   GameObject& m_parent;
};
