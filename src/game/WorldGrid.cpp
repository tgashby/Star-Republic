//
//  WorldGrid.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>

#include "WorldGrid.h"

#define MAGNETIC_TIME_MOVEMENT 3000

/*
 TODO:
 - Figure out why determineQuadrant isn't working...
 - Update moving objects
 - Move GameEngine stuff into here/integrate with GameEngine (Done?)
 - Prob lots more...
 */

WorldGrid::WorldGrid(Path& path, WorldData& world, Modules* modules, Player* player, vector<Bullet*>* bulletList, vector<Missile*>* missileList)
   : m_path(path), m_world(world), m_modules(modules), m_player(player)
{
   m_missileList = missileList;
   m_bulletList = bulletList;
   m_currentQuadrant = 0;
   m_shouldUpdate = true;
   
   makeGrid();
}

WorldGrid::~WorldGrid()
{
   
}

Quadrant WorldGrid::getCurrentQuadrant()
{
   return m_path.getCurrentQuadrant();
}

void WorldGrid::tic(uint64_t dt)
{
   GameObject* currentClosest = NULL;

   Quadrant quad = m_path.getCurrentQuadrant();
   
   vec3 closestdir = vec3(10000, 10000, 10000);

   for (vector<Bullet*>::iterator i = m_bulletList->begin();
	i != m_bulletList->end(); i++) {
      (*i)->tic(dt);
   }
   
   for (vector<Missile*>::iterator i = m_missileList->begin();
	i != m_missileList->end(); i++) {
      (*i)->tic(dt);
   }

   for (list<GameObject*>::iterator i = quad.m_gameObjects.begin(); 
	i != quad.m_gameObjects.end(); i++) 
   {
      GameObject* currObj = *i;
      
      currObj->tic(dt);
      
      vec3 dirToPlayer = (*i)->getPosition() - m_player->getPosition();
      
      if (typeid(**i) == typeid(Turret)) 
      {
         Turret* turret = ((Turret*)*i);
         
         if (closestdir.Length() > dirToPlayer.Length() 
             && dirToPlayer.Normalized()
	     .Dot(m_player->getAimForward().Normalized()) > 0.96
             && turret->isAlive()) {
            closestdir = dirToPlayer;
	    currentClosest = turret;
	 }
         
         // Turret not currently firing, but I think it's because
         // the player starts too close to the turret
         if (turret->isAlive() && dirToPlayer.Length() < 1500 && turret->shouldFire())
         {
            vec3 dirToPlayerNorm = dirToPlayer.Normalized();

            Bullet* bullet = 
            new Bullet("models/lance.obj", "textures/red_texture.bmp", 
                       m_modules, turret->getHeadPosition(), 
                       -dirToPlayerNorm, 
                       dirToPlayerNorm.Cross(turret->getPosition()), *(*i),
                       Bullet::defaultTimeToLive, 0.7f);
            
            m_modules->renderingEngine->addObject3d(bullet);
            //m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
            m_bulletList->push_back(bullet);
         }
      }

      if (typeid(**i) == typeid(Objective)) {
	 Objective* objective = ((Objective*)*i);
	 vec3 dirObjToPlayer = objective->getPosition() 
	    - m_player->getPosition();
	 
	 if (closestdir.Length() > dirObjToPlayer.Length()
	     && dirObjToPlayer.Normalized().Dot(m_player->getAimForward().Normalized()) > .96 && objective->isAlive()) {
	    closestdir = dirObjToPlayer;
	    currentClosest = objective;
	 }
      }
      
      if (typeid(**i) == typeid(EnemyShip)) 
      {
         EnemyShip* enemyShip = ((EnemyShip*)*i);
         
         vec3 dirEnemyToPlayer = enemyShip->getPosition() 
	    - m_player->getPosition();
         
         if (closestdir.Length() > dirEnemyToPlayer.Length()
             && dirEnemyToPlayer.Normalized().Dot(m_player->getAimForward().Normalized()) > 0.96
             && enemyShip->isAlive()) {
            closestdir = dirEnemyToPlayer;
	    currentClosest = enemyShip;
	 }
         
         if (dirEnemyToPlayer.Length() < 700 && enemyShip->shouldFire() && enemyShip->isAlive())
         {
            vec3 dirToPlayerNorm = dirEnemyToPlayer.Normalized();

            Bullet* bullet = 
            new Bullet("models/lance.obj", "textures/red_texture.bmp", 
                       m_modules, enemyShip->getLeftCannonPos(), 
                       enemyShip->getAimForward(), 
                       dirToPlayerNorm.Cross(enemyShip->getLeftCannonPos()), 
                       *enemyShip, Bullet::defaultTimeToLive, 0.6f);
            
            m_modules->renderingEngine->addObject3d(bullet);
            //m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
            m_bulletList->push_back(bullet);
            
            bullet = 
            new Bullet("models/lance.obj", "textures/red_texture.bmp", 
                       m_modules, enemyShip->getRightCannonPos(), 
                       enemyShip->getAimForward(), 
                       dirToPlayerNorm.Cross(enemyShip->getRightCannonPos()), 
                       *enemyShip, Bullet::defaultTimeToLive, 0.6f);
            
            m_modules->renderingEngine->addObject3d(bullet);
            //m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
            m_bulletList->push_back(bullet);
         }
      }
      
      if (typeid(**i) == typeid(EnemyGunship)) 
      {
         EnemyGunship* enemyShip = ((EnemyGunship*)*i);
         
         vec3 dirEnemyToPlayer = enemyShip->getPosition() - m_player->getPosition();
         
         if (closestdir.Length() > dirEnemyToPlayer.Length()
             && dirEnemyToPlayer.Normalized().Dot(m_player->getAimForward().Normalized()) > 0.96
             && enemyShip->isAlive()) {
            closestdir = dirEnemyToPlayer;
	    currentClosest = enemyShip;
	 }
         
         if (dirEnemyToPlayer.Length() < 1600 &&
             (enemyShip->shouldFire1() || enemyShip->shouldFire2()) && enemyShip->isAlive())
         {

            vec3 dirToPlayerNorm = dirEnemyToPlayer.Normalized();
            
            if (enemyShip->shouldFire1())
            {
               Bullet* bullet = 
               new Bullet("models/lance.obj", "textures/red_texture.bmp", 
                          m_modules, enemyShip->getLeftCannonPos(), 
                          enemyShip->getAimForward(), 
                          dirToPlayerNorm.Cross(enemyShip->getLeftCannonPos()), 
                          *enemyShip, Bullet::defaultTimeToLive, 0.6f);
               
               m_modules->renderingEngine->addObject3d(bullet);
               //m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
               m_bulletList->push_back(bullet);
            }
            
            if (enemyShip->shouldFire2())
            {
               Bullet* bullet = 
               new Bullet("models/lance.obj", "textures/red_texture.bmp", 
                          m_modules, enemyShip->getRightCannonPos(), 
                          enemyShip->getAimForward(), 
                          dirToPlayerNorm.Cross(enemyShip->getRightCannonPos()), 
                          *enemyShip, Bullet::defaultTimeToLive, 0.5f);
               
               m_modules->renderingEngine->addObject3d(bullet);
               //m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
               m_bulletList->push_back(bullet);
            }
         }
      }   
   }
   
   if (currentClosest != prevClosest) {
      magneticTimer = 0;
   }
   
   magneticTimer += dt;
   if (magneticTimer > MAGNETIC_TIME_MOVEMENT) {
      magneticTimer = MAGNETIC_TIME_MOVEMENT;
   }

   float percentTimer = (magneticTimer * 1.0) / (MAGNETIC_TIME_MOVEMENT * 1.0);
   vec3 prevForward = m_player->getMagneticForward();
   vec3 currForward;
   
   if (closestdir.x != 10000 && closestdir.y != 10000 && closestdir.z != 10000) {
      currForward = closestdir.Normalized();
   }
   else {
      currForward = m_player->getAimForward().Normalized();
   }
   
   vec3 tempForward = (prevForward * (1.0 - percentTimer)) + (currForward * percentTimer);

   m_player->setMagneticForward(tempForward);

   prevClosest = currentClosest;

   /*if (closestdir.x != 10000 && closestdir.y != 10000 && closestdir.z != 10000)
      m_player->setMagneticForward(closestdir.Normalized());
   else
   m_player->setMagneticForward(m_player->getAimForward().Normalized());*/
}

void WorldGrid::checkCollisions()
{
   Quadrant quad = m_path.getCurrentQuadrant();
   vector<GameObject *> temp;

   //Creates a new list of the items in the quad
   temp.insert(temp.end(), quad.m_gameObjects.begin(), 
	       quad.m_gameObjects.end());
   //Adds the bullets to the list to check
   temp.insert(temp.end(), m_bulletList->begin(),
	       m_bulletList->end());

   //Adds the missiles to the list to check
   temp.insert(temp.end(), m_missileList->begin(),
	       m_missileList->end());
   
   for (vector<GameObject*>::iterator i = temp.begin(); i != temp.end(); i++) 
   {
      GameObject* currObj = *i;
      
      for (vector<GameObject*>::iterator j = i; j != temp.end(); j++) 
      {
         // HACK to get iterators to work
         if (j != i) 
         {
            GameObject* toCheck = *j;
            
            // Check and do collisions
            if (currObj->isAlive() && toCheck->isAlive()
                && currObj->collidesWith(*toCheck)) 
            {
               currObj->doCollision(*toCheck);
               toCheck->doCollision(*currObj);
            }
            
            if (toCheck->isAlive() && m_player->collidesWith(*toCheck)) 
            {
               m_player->doCollision(*toCheck);
               toCheck->doCollision(*m_player);
            }
         }
      }
   }
}

std::list<IObject3d*> WorldGrid::getDrawableObjects()
{

   list<IObject3d*> temp;
   list<IObject3d*> objs = m_path.getCurrentQuadrant().m_obj3Ds;
   temp.insert(temp.end(), objs.begin(), objs.end());
   temp.insert(temp.end(), m_bulletList->begin(), m_bulletList->end());
   temp.insert(temp.end(), m_missileList->begin(), m_missileList->end());
   return temp;
   //return m_path.getCurrentQuadrant().m_obj3Ds;
}

void WorldGrid::makeGrid()
{
   // Connect the points into quadrants
   for (std::vector<PathPointData>::iterator i = m_world.path.begin(); i < m_world.path.end(); i++) 
   {
      // For all the units in the quadrant
      for (std::vector<UnitData>::iterator j = (*i).units.begin(); j != (*i).units.end(); j++) 
      {
         UnitData currUnit = *j;
         
         Vector3<float> position = currUnit.loc;
         Vector3<float> forward = currUnit.fwd;
         Vector3<float> up = currUnit.up;
         
         switch (currUnit.type) 
         {
            case UNIT_TURRET:
               Turret* newTurret = new Turret(m_player, m_modules);
               
               newTurret->setPosition(position);
               newTurret->setForward(forward);
               newTurret->setUp(up);
               
               m_path.addToQuadrants(position, newTurret, newTurret);
               m_modules->renderingEngine->addObject3d(newTurret);
               break;
         }
      }
      
      // Scene Objects
      for (std::vector<PropData>::iterator j = (*i).props.begin(); j != (*i).props.end(); j++) 
      {
         PropData currProp = *j;
         
         vec3 position = currProp.loc;
         vec3 forward = currProp.fwd;
         vec3 up = currProp.up;
         
         if (currProp.name.find("Path") != string::npos) {
            PathObject* pathObj = new PathObject("models/" + currProp.name + ".obj", "textures/" + currProp.name + ".bmp", m_modules, position, forward, up);
            m_path.addToQuadrants(position, pathObj, pathObj);
            m_modules->renderingEngine->addObject3d(pathObj);
         }
         else {
            SceneObject* sceneObj = new SceneObject("models/" + currProp.name + ".obj", "textures/" + currProp.name + ".bmp", position, forward, up, m_modules);
            m_path.addToQuadrants(position, NULL, sceneObj);
            m_modules->renderingEngine->addObject3d(sceneObj);
         }
      }
   }
}
