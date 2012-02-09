//
//  Explodeable.cpp
//  StarRepub
//
//  Created by Taggart Ashby on 2/9/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>

#include "Explodeable.h"

Explodeable::Explodeable(string meshName, vec3 position, Modules* modules)
: Object3d()
{
   m_mesh = new Mesh("sphere.obj", "fire.bmp", modules);
   m_meshList.push_back(m_mesh);
   
   m_position = position;
   m_mesh->setModelMtx(mat4::Translate(m_position.x, m_position.y, m_position.z));
}

void Explodeable::explode()
{
   mat4 newModelMtx = m_mesh->getModelMtx();
   
   switch (state) 
   {
      case START:
         // small
         newModelMtx *= mat4::Scale(0.3f);
         break;
         
      case MIDDLE:
         // Big
         newModelMtx *= mat4::Scale(2.0f);
         break;
         
      case END:
         //small
         newModelMtx *= mat4::Scale(0.3f);
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
   else if (stateTimer >= 2000)
   {
      state = END;
   }
   else if (stateTimer > 3000)
   {
      state = DONE;
   }
}