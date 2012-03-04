//
//  WorldGrid.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>

#include "WorldGrid.h"

/*
 TODO:
 - Figure out why determineQuadrant isn't working...
 - Update moving objects
 - Move GameEngine stuff into here/integrate with GameEngine (Done?)
 - Prob lots more...
 */

WorldGrid::WorldGrid(Path& path, WorldData& world, Modules* modules, Player* player)
   : m_path(path), m_world(world), m_modules(modules), m_player(player)
{
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

void WorldGrid::tic(uint64_t dt, std::vector<Bullet*>* m_bulletList)
{
   Quadrant quad = m_path.getCurrentQuadrant();
   
   for (std::list<GameObject*>::iterator i = quad.m_gameObjects.begin(); i != quad.m_gameObjects.end(); i++) 
   {
      GameObject* currObj = *i;
      
      currObj->tic(dt);
      
      vec3 closestdir = vec3(10000, 10000, 10000);
      
      vec3 dirToPlayer = (*i)->getPosition() - m_player->getPosition();
      
      if (typeid(**i) == typeid(Turret)) 
      {
         Turret* turret = ((Turret*)*i);
         
         if (closestdir.Length() > dirToPlayer.Length() 
             && dirToPlayer.Normalized().Dot(m_player->getAimForward().Normalized()) > 0.96
             && turret->isAlive())
            closestdir = dirToPlayer;
         
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
            m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
            m_bulletList->push_back(bullet);
         }
      }
      
      if (typeid(**i) == typeid(EnemyShip)) 
      {
         EnemyShip* enemyShip = ((EnemyShip*)*i);
         
         vec3 dirEnemyToPlayer = enemyShip->getPosition() - m_player->getPosition();
         
         if (closestdir.Length() > dirEnemyToPlayer.Length()
             && dirEnemyToPlayer.Normalized().Dot(m_player->getAimForward().Normalized()) > 0.96
             && enemyShip->isAlive())
            closestdir = dirEnemyToPlayer;
         
         if (dirEnemyToPlayer.Length() < 700 && enemyShip->shouldFire())
         {
            vec3 dirToPlayerNorm = dirEnemyToPlayer.Normalized();
            
            Bullet* bullet = 
            new Bullet("models/lance.obj", "textures/red_texture.bmp", 
                       m_modules, enemyShip->getLeftCannonPos(), 
                       enemyShip->getAimForward(), 
                       dirToPlayerNorm.Cross(enemyShip->getLeftCannonPos()), 
                       *enemyShip, Bullet::defaultTimeToLive, 0.6f);
            
            m_modules->renderingEngine->addObject3d(bullet);
            m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
            m_bulletList->push_back(bullet);
            
            bullet = 
            new Bullet("models/lance.obj", "textures/red_texture.bmp", 
                       m_modules, enemyShip->getRightCannonPos(), 
                       enemyShip->getAimForward(), 
                       dirToPlayerNorm.Cross(enemyShip->getRightCannonPos()), 
                       *enemyShip, Bullet::defaultTimeToLive, 0.6f);
            
            m_modules->renderingEngine->addObject3d(bullet);
            m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
            m_bulletList->push_back(bullet);
         }
      }
      
      if (typeid(**i) == typeid(EnemyGunship)) 
      {
         EnemyGunship* enemyShip = ((EnemyGunship*)*i);
         
         vec3 dirEnemyToPlayer = enemyShip->getPosition() - m_player->getPosition();
         
         if (closestdir.Length() > dirEnemyToPlayer.Length()
             && dirEnemyToPlayer.Normalized().Dot(m_player->getAimForward().Normalized()) > 0.96
             && enemyShip->isAlive())
            closestdir = dirEnemyToPlayer;
         
         if (dirEnemyToPlayer.Length() < 1600 &&
             (enemyShip->shouldFire1() || enemyShip->shouldFire2()))
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
               m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
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
               m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
               m_bulletList->push_back(bullet);
            }
         }
      }   
      
      
      if (closestdir.x != 10000 && closestdir.y != 10000 && closestdir.z != 10000)
         m_player->setMagneticForward(closestdir.Normalized());
      else
         m_player->setMagneticForward(m_player->getAimForward().Normalized());
   }
}

void WorldGrid::checkCollisions()
{
   Quadrant quad = m_path.getCurrentQuadrant();
   
   for (std::list<GameObject*>::iterator i = quad.m_gameObjects.begin(); i != quad.m_gameObjects.end(); i++) 
   {
      GameObject* currObj = *i;
      
      for (std::list<GameObject*>::iterator j = i; j != quad.m_gameObjects.end(); j++) 
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
   return m_path.getCurrentQuadrant().m_obj3Ds;
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
         
         SceneObject* sceneObj = new SceneObject("models/" + currProp.name + ".obj", "textures/" + currProp.name + ".bmp", position, forward, up, m_modules);
         
         m_path.addToQuadrants(position, NULL, sceneObj);
         m_modules->renderingEngine->addObject3d(sceneObj);
      }
   }
}