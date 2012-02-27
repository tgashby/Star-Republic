//
//  Turret.cpp
//  StarRepub
//
//  Created by Taggart Ashby on 2/3/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include "Turret.h"

#define TURRETHEADHEIGHT 35

/*
Turret::Turret(Player& player, string headName, string headTexture, string midName, string midTexture, string footName, string footTexture, Modules *modules) 
  : Enemy(player), Explodeable(vec3(0,0,0), _TURRET_DEFAULT_EXPLOSION_RADIUS, modules)
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
}*/

/*
"models/turrethead.obj", 
"textures/turretHeadTex.bmp", "models/turretMid.obj", 
"textures/turretMidTex.bmp", "models/turretbase.obj", 
"textures/turretBaseTex.bmp"*/

Turret::Turret(Player* player, Modules *modules) : Enemy(player), Explodeable(vec3(0,0,0), _TURRET_DEFAULT_EXPLOSION_RADIUS, modules) {
   m_footMesh = new Mesh("models/turretbase.obj", "textures/turretBaseTex.bmp", modules);
   m_footMesh->setScale(_TURRET_SCALE);
   m_meshList.push_back(m_footMesh);
   mat4 modelMtx = mat4::Translate(0, 0, 0);
   m_footMesh->setModelMtx(modelMtx);
   
   m_midMesh = new Mesh("models/turretMid.obj", "textures/turretMidTex.bmp", modules);
   m_midMesh->setScale(_TURRET_SCALE);
   m_meshList.push_back(m_midMesh);
   modelMtx *= mat4::Translate(0, 1, 0);
   m_midMesh->setModelMtx(modelMtx);
   
   m_headMesh = new Mesh("models/turrethead.obj", "textures/turretheadtex.bmp", modules);
   m_headMesh->setScale(_TURRET_SCALE);
   m_meshList.push_back(m_headMesh);
   //modelMtx *= mat4::Translate(0, 13, 0);
   m_headMesh->setModelMtx(modelMtx);
   
   firing = false;
   firingTimer = 0;
}

Turret::~Turret()
{
   
}


void Turret::tic(uint64_t time)
{
   // Rotate head toward player
   // aim
   // fire
   //mat4 modelMtx = mat4::Translate(m_position.x, m_position.y, m_position.z);
  if (isAlive()) {
     mat4 modelMtx = mat4::Rotate(90, vec3(1, 0, 0)) * mat4::Magic(m_forward, m_up, m_position);
    m_footMesh->setModelMtx(modelMtx);
   
    vec3 dirToPlayer = (m_playerRef.getPosition() - m_position).Normalized();
    vec3 intermed = dirToPlayer.Cross(m_up);
    vec3 dirToPlayerFlat = intermed.Cross(m_up);
   
    modelMtx = mat4::Rotate(90, vec3(1, 0, 0)) * mat4::Magic(-dirToPlayerFlat, m_up, m_position);
    m_midMesh->setModelMtx(modelMtx);
   
    modelMtx = mat4::Rotate(90, vec3(1, 0, 0)) * 
               mat4::Magic(-dirToPlayer, m_up, getHeadPosition());
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
  else {
    explosionTic(time);
  }
}

Vector3<float> Turret::getHeadPosition()
{
   vec3 toRet = m_position + (m_up.Normalized() * TURRETHEADHEIGHT);
   
   return toRet;
}

void Turret::doCollision(GameObject & other) {
   //Do collision stuff here!
   
  if (isAlive()) {
    if (typeid(other) == typeid(Bullet)) 
      {
	collideWith(((Bullet&)other));
      }
    
    if (typeid(other) == typeid(Player)) 
      {
	collideWith(((Player&)other));
      }
    
    if (typeid(other) == typeid(Enemy)) 
      {
	collideWith(((Enemy&)other));
      }
    
    if (typeid(other) == typeid(Missile)) {
      collideWith(((Missile&)other));
    }
    
    if (m_health <= 0) {
      m_alive = false;
      
      m_headMesh->setVisible(false);
      m_midMesh->setVisible(false);
      m_footMesh->setVisible(false);
      
      setExplosionPosition(m_position);
    }
  }
}

bool Turret::shouldFire() {
   return firing;
}

void Turret::collideWith(Bullet& bullet) {
   if (&bullet.getParent() != this) {
      m_health -= 100;
   }
}

void Turret::collideWith(Player& player) {
  m_health -= 100;
}

void Turret::collideWith(Enemy& enemy) {
   
}

void Turret::collideWith(Missile& missile) {
  m_health -= 100;
}

vec3 Turret::getPosition() {
  return m_position;
}
