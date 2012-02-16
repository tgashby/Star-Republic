//
//  Explodeable.cpp
//  StarRepub
//
//  Created by Taggart Ashby on 2/9/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>

#include "Explodeable.h"

Explodeable::Explodeable(vec3 position, Modules* modules)
: Object3d()
{
   m_explosionMesh1 = new Mesh("models/enemy.obj", "textures/test5.bmp", modules);
   m_meshList.push_back(m_explosionMesh1);
   
   m_explosionPos = position;
   m_explosionMesh1->setModelMtx(mat4::Scale(0));

   m_duration = _DEF_EXPLOSION_ANIMATION_TIME;
   m_radius = _DEF_EXPLOSION_RADIUS;
}

Explodeable::Explodeable(vec3 position, int duration, float radius, 
			 Modules* modules) : Object3d()
{
   m_explosionMesh1 = new Mesh("models/enemy.obj", "textures/test5.bmp", modules);
   m_meshList.push_back(m_explosionMesh1);

   m_explosionPos = position;
   m_explosionMesh1->setModelMtx(mat4::Scale(0));

   m_duration = duration;
   m_radius = radius;
}

void Explodeable::setExplodePosition(vec3 pos) {
   m_explosionPos = pos;
}

void Explodeable::tic(uint64_t dt) {
  explosionTic(dt);
}

void Explodeable::explosionTic(uint64_t dt)
{
   //Going to be small, then jump to large eventually
   m_stateTimer += dt;

   //FOR DEBUGGING
   if (m_stateTimer > m_duration) {
      m_stateTimer = m_duration;
      m_explosionMesh1->setVisible(false);
   }
   
   float modelScale = m_radius * m_stateTimer / m_duration;
   
   m_explosionMesh1->setModelMtx(mat4::Scale(modelScale) 
    * mat4::Translate(m_explosionPos.x, m_explosionPos.y, m_explosionPos.z));
}
