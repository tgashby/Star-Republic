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
   static const float defaultSpeed=1;
   //Default collision radius
   static const float defaultBulletRadius = 1;

   Bullet(string fileName, string textureName, Modules *modules, 
    Vector3<float> pos, Vector3<float> forw, Vector3<float> up,
    const uint64_t timeToLive=Bullet::defaultTimeToLive,
    const float speed=Bullet::defaultSpeed ); 
   ~Bullet();

   void tic(uint64_t time);
   Vector3<float> getPosition();
   Vector3<float> getForward();
   Vector3<float> getUp();
   void calculateSide();
   bool isAlive();
   void doCollision(GameObject & other);

 private:

   Mesh* m_mesh;
   uint64_t m_lifetime; 
   uint64_t m_timeToLive; 
   bool m_alive;
   float m_speed;
   

   Vector3<float> m_position;
   Vector3<float> m_forward;
   Vector3<float> m_up;
   Vector3<float> m_side;
};
