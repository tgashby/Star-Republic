//
//  Bullet.cpp
//  StarRepub
//
//  Created by Chad Brantley on 2/7/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include "Bullet.h"


Bullet::Bullet(string fileName, string textureName, Modules *modules, 
	       Vector3<float> pos, Vector3<float> forw, Vector3<float> up, GameObject& parent,
          const uint64_t timeToLive, 
               const float speed) 
  : Object3d(),GameObject(pos,forw.Normalized() * speed,forw.Normalized(),
     vec3(0,0,0), m_up, defaultBulletRadius, 1), m_parent(parent)
{
   m_position = pos;
   m_forward = forw.Normalized();
   m_up = up.Normalized();
   m_speed=speed; 


   //We should have a better way of doing this. We don't need to load the
   //bullet mesh for every bullet. And, there are going to be a lot of a bullets
   m_mesh = new Mesh(fileName, textureName, modules);
   m_meshList.push_back(m_mesh);

   mat4 modelMtx = mat4::Magic(m_forward, m_up, m_position);
   m_mesh->setModelMtx(modelMtx);
   
   m_lifetime = 0;
}

Bullet::~Bullet() 
{
   delete m_mesh;
}

void Bullet::tic(uint64_t time)
{  
   mat4 modelMtx;
   //Add position to global velocity
   m_position = m_position + (m_forward * time * m_speed);

   modelMtx = mat4::Magic(m_forward, m_up, m_position);
   m_mesh->setModelMtx(modelMtx);
   
   m_lifetime += time;
   if(m_lifetime > m_timeToLive){
      m_alive = false;
   } 

}

Vector3<float> Bullet::getPosition() {
  return m_position;
}

Vector3<float> Bullet::getForward() {
  return m_forward;
}

Vector3<float> Bullet::getUp() {
  return m_up;
}

void Bullet::doCollision(GameObject & other) {
   //The Bullet dies!
   
   if (m_mesh->isVisible())
   {
      if (&other != &m_parent) 
      {
         m_alive = false;
         m_mesh->setVisible(false);
      }
   }
}
