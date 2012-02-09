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
   modelMtx *= mat4::Translate(0, 1, 0);
   m_midMesh->setModelMtx(modelMtx);
   
   m_headMesh = new Mesh(headName, headTexture, modules);
   m_meshList.push_back(m_headMesh);
   modelMtx *= mat4::Translate(0, 13, 0);
   m_headMesh->setModelMtx(modelMtx);
   
   firing = false;
   firingTimer = 0;
}

Turret::~Turret()
{
//   for (std::list<Bullet*>::iterator i = m_bullets.begin(); i != m_bullets.end(); i++) {
//      Bullet* temp = *i;
//      
//      delete temp;
//   }
}


void Turret::tic(uint64_t time)
{
   // Rotate head toward player
   // aim
   // fire
   mat4 modelMtx = mat4::Translate(m_position.x, m_position.y, m_position.z);
   m_footMesh->setModelMtx(modelMtx);
   
   modelMtx *= mat4::Translate(0, 1, 0);
   m_midMesh->setModelMtx(modelMtx);
   
   modelMtx *= mat4::Translate(0, 13, 0);
   m_headMesh->setModelMtx(modelMtx);
   
   vec3 dirToPlayer = (m_playerRef.getPosition() - m_position).Normalized();
   vec3 intermed = dirToPlayer.Cross(m_up);
   vec3 dirToPlayerFlat = intermed.Cross(m_up);
   
   modelMtx = mat4::Magic(-dirToPlayerFlat, m_up, m_position);
   m_midMesh->setModelMtx(modelMtx);
   
   modelMtx = mat4::Magic(-dirToPlayer, m_up, m_position);
   modelMtx *= mat4::Translate(0, 13, 0);
   m_headMesh->setModelMtx(modelMtx);
   
   firingTimer += time;
   
   if (firingTimer > 400)
   {
      firing = true;
      firingTimer = 0;
   }
   else 
   {
      firing = false;
   }
}

Vector3<float> Turret::getHeadPosition()
{
   vec3 toRet = m_position + (m_up.Normalized() * 13);
   
   return toRet;
}

void Turret::doCollision(GameObject & other){
   //Do collision stuff here!
} 

bool Turret::shouldFire()
{
   return firing;
}

void Turret::collideWith(Bullet& bullet)
{
   health -= 25;
   
   // bullet.removeMe()
   
   if (health <= 0) 
   {
      // alive = false;
   }
}

void Turret::collideWith(Player& player)
{
   
}

void Turret::collideWith(Enemy& enemy)
{
   
}
