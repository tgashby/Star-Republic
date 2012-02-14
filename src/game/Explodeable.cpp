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
   m_mesh = new Mesh("models/enemy.obj", "textures/test5.bmp", modules);
   m_meshList.push_back(m_mesh);
   
   m_position = position;
   m_mesh->setModelMtx(mat4::Scale(0));

   m_duration = _DEF_EXPLOSION_ANIMATION_TIME;
   m_radius = _DEF_EXPLOSION_RADIUS;
}

Explodeable::Explodeable(vec3 position, int duration, float radius, 
			 Modules* modules) : Object3d()
{
   m_mesh = new Mesh("models/enemy.obj", "textures/test5.bmp", modules);
   m_meshList.push_back(m_mesh);

   m_position = position;
   m_mesh->setModelMtx(mat4::Scale(0));

   m_duration = duration;
   m_radius = radius;
}

void Explodeable::setPosition(vec3 pos) {
   m_position = pos;
}

void Explodeable::tic(uint64_t dt)
{
   //Going to be small, then jump to large eventually
   m_stateTimer += dt;

   //FOR DEBUGGING
   if (m_stateTimer > m_duration) {
      m_stateTimer = m_duration;
      m_mesh->setVisible(false);
   }
   
   float modelScale = m_radius * m_stateTimer / m_duration;
   
   m_mesh->setModelMtx(mat4::Scale(modelScale) 
    * mat4::Translate(m_position.x, m_position.y, m_position.z));
}
