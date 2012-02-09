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
   m_mesh = new Mesh("models/cube.obj", "textures/test4.bmp", modules);
   m_meshList.push_back(m_mesh);
   
   m_position = position;
   m_mesh->setModelMtx(mat4::Translate(m_position.x, m_position.y, m_position.z));
   
   state = START;
}

void Explodeable::explode()
{
   mat4 newModelMtx = mat4::Translate(m_position.x, m_position.y, m_position.z);
   
   switch (state) 
   {
      case START:
         // small
         newModelMtx *= mat4::Scale(25.3f);
         break;
         
      case MIDDLE:
         // Big
         newModelMtx *= mat4::Scale(200.0f);
         break;
         
      case END:
         //small
         newModelMtx *= mat4::Scale(25.3f);
         break;
         
      case DONE:
         //disappear
         newModelMtx *= mat4::Scale(0.0f);
         break;
         
      default:
         std::cerr << "This shouldn't happen... Explodeable\n";
         break;
   }
   
   m_mesh->setModelMtx(newModelMtx);
}

void Explodeable::tic(uint64_t dt)
{
   stateTimer += dt;
   
   if (stateTimer > 1000 && stateTimer < 2000) 
   {
      state = MIDDLE;
   }
   else if (stateTimer >= 2000 && stateTimer < 3000)
   {
      state = END;
   }
   else if (stateTimer > 3000)
   {
      state = DONE;
   }
   
   explode();
}

void Explodeable::setPosition(Vector3<float> position)
{
   m_position = position;
}