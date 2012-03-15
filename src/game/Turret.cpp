//
//  Turret.cpp
//  StarRepub
//
//  Created by Taggart Ashby on 2/3/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include "Turret.h"

#define TURRETHEADHEIGHT 35
#define TURRET_RADIUS 40

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

   flashtimer = 0;

   m_health = 150;
}

Turret::~Turret()
{
   
}


void Turret::tic(uint64_t time)
{
  if (isAlive())
  {
     if (flashtimer != 0)
     {
        m_footMesh->setVisible(false);
        m_midMesh->setVisible(false);
        m_headMesh->setVisible(false);
        flashtimer--;
     }
     else
     {
        m_footMesh->setVisible(true);
        m_midMesh->setVisible(true);
        m_headMesh->setVisible(true);
     }
  }

   // Rotate head toward player
   // aim
   // fire
   //mat4 modelMtx = mat4::Translate(m_position.x, m_position.y, m_position.z);
  if (isAlive()) {
     mat4 modelMtx = mat4::Rotate(90, vec3(1, 0, 0)) * mat4::Magic(m_forward, m_up, m_position);
    m_footMesh->setModelMtx(modelMtx);
   
    vec3 target = getTargetPosition();
    vec3 dirToPlayer = (target - getHeadPosition()).Normalized();
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

Vector3<float> Turret::getTargetPosition()
{
   float distance = (m_playerRef->getPosition() - getHeadPosition()).Length();
   float time = (distance/0.6f)/2.0f;
   vec3 target = m_playerRef->getPosition() + (m_playerRef->getShipVelocity() * time);
  /* cerr << "A PLAYER GOES X: " << m_playerRef->getPosition().x << " Y: " << m_playerRef->getPosition().y << " Z: " << m_playerRef->getPosition().z << "\n";
   cerr << "A TARGET GOES X: " << target.x << " Y: " << target.y << " Z: " << target.z << "\n";
   cerr << "TIME IS: " << time << "\n"; */
   return target;
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
      m_health -= 25;
      if (flashtimer == 0)
         flashtimer = 2; 
   }
}

void Turret::collideWith(Player& player) {
  m_health -= 100;
}

void Turret::collideWith(Enemy& enemy) {
   
}

void Turret::collideWith(Missile& missile) {
  m_health -= 100;
  if (flashtimer == 0)
     flashtimer = 2; 
}

vec3 Turret::getPosition() {
  return m_position;
}

bool Turret::viewCull(vector<vec4> *planes) {
   vector<vec4>::iterator plane = planes->begin();
   float val;
   
   for (; plane != planes->end(); ++plane) {
      val = plane->x * m_position.x +
      plane->y * m_position.y +
      plane->z * m_position.z +
      plane->w;
      if (val < -TURRET_RADIUS) {
         return false;
      }
   }
   return true;
}
