//
//  Turret.cpp
//  StarRepub
//
//  Created by Taggart Ashby on 2/3/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include "Turret.h"

Turret::Turret(Player& player, string headName, string headTexture, string midName, string midTexture, string footName, string footTexture, Modules *modules) 
: Object3d(), Enemy(player), health(100)
{
   m_footMesh = new Mesh(footName, footTexture, modules);
   m_meshList.push_back(m_footMesh);
   mat4 modelMtx = mat4::Translate(0, 0, 0);
   m_footMesh->setModelMtx(modelMtx);
   
   
   m_midMesh = new Mesh(midName, midTexture, modules);
   m_meshList.push_back(m_midMesh);
   modelMtx = mat4::Translate(0, 1, 0);
   m_midMesh->setModelMtx(modelMtx);
   
   m_headMesh = new Mesh(headName, headTexture, modules);
   m_meshList.push_back(m_headMesh);
   modelMtx = mat4::Translate(0, 13, 0);
   m_headMesh->setModelMtx(modelMtx);
}

Turret::~Turret()
{
   
}


void Turret::tic(uint64_t time)
{
   // Rotate head toward player
   // aim
   // fire
   
   vec3 d(0,0,0);
   d.x = m_position.x - (m_playerRef.getPosition().x + 6);
   d.y = m_position.y - (m_playerRef.getPosition().y + 3);
   d.z = m_position.z - (m_playerRef.getPosition().z + 3);
   
   vec3 Rotation(0,0,0);
   
   //Inclination?
   if((m_playerRef.getPosition().y + 3) > 0){    
      Rotation.x = (atan(sqrt(d.x * d.x + d.z * d.z) / d.y) * 180 / 3.1415926); 
   }
   else {
      Rotation.x = (-atan(sqrt(d.x * d.x + d.z * d.z) / d.y) * 180 / 3.1415926); 
   }
   
   //Rotation?
   if (d.x < 0) 
   {
         if (d.z < 0)
         Rotation.y = -(atan(d.x / d.z) * 180 / 3.1415926);
      else
         Rotation.y = (atan(d.x / d.z) * 180 / 3.1415926); 
   }
   else
   {
      if (d.z < 0)
         Rotation.y = -(atan(d.x / d.z) * 180 / 3.1415926);
      else
         Rotation.y = (atan(d.x / d.z) * 180 / 3.1415926);  
   }
   
   m_headMesh->setModelMtx(mat4::Rotate(Rotation));
}